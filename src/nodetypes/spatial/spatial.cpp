#include "spatial.hpp"
#include "../../globals.h"

#include "../../core/client.hpp"
#include "../../core/scenegraph.hpp"
#include "../../interfaces/spatial.hpp"
#include "../../nodetypes/core/alias.hpp"
#include "stereokit.h"
#include <string>
#include <vector>
using namespace StardustXR;
namespace StardustXRServer {

Spatial::Spatial(Client *client, matrix transformMatrix) : Node(client, false) {
	setTransformMatrix(transformMatrix);
}
Spatial::Spatial(Client *client, Spatial *spatialParent, matrix transformMatrix, bool translatable, bool rotatable, bool scalable, bool zoneable) :
		Spatial(client, transformMatrix) {
	this->spatialParent = spatialParent;
	this->translatable = translatable;
	this->rotatable = rotatable;
	this->scalable = scalable;
	this->zoneable = zoneable;

	STARDUSTXR_NODE_METHOD("move", &Spatial::move)
	STARDUSTXR_NODE_METHOD("rotate", &Spatial::rotate)
	STARDUSTXR_NODE_METHOD("scale", &Spatial::scaleThis)

	STARDUSTXR_NODE_METHOD("getTransform", &Spatial::getTransform)

	STARDUSTXR_NODE_METHOD("setOrigin", &Spatial::setOrigin)
	STARDUSTXR_NODE_METHOD("setOrientation", &Spatial::setOrientation)
	STARDUSTXR_NODE_METHOD("setScale", &Spatial::setScale)

	STARDUSTXR_NODE_METHOD("setPose", &Spatial::setPose)
	STARDUSTXR_NODE_METHOD("setTransform", &Spatial::setTransform)

	STARDUSTXR_NODE_METHOD("setSpatialParent", &Spatial::setSpatialParentFlex)
	STARDUSTXR_NODE_METHOD("setSpatialParentInPlace", &Spatial::setSpatialParentInPlaceFlex)

	STARDUSTXR_NODE_METHOD("setZoneable", &Spatial::setZoneable)

	std::lock_guard<std::mutex> lock(SpatialInterface::spatialMutex);
	SpatialInterface::spatials.push_back(this);
}
Spatial::Spatial(Client *client, Spatial *spatialParent, vec3 position, quat rotation, vec3 scale, bool translatable, bool rotatable, bool scalable, bool zoneable) :
	Spatial(client, spatialParent, matrix_trs(position, rotation, scale), translatable, rotatable, scalable, zoneable) {}
Spatial::~Spatial() {
	std::lock_guard<std::mutex> lock(SpatialInterface::spatialMutex);
	SpatialInterface::spatials.erase(std::remove(SpatialInterface::spatials.begin(), SpatialInterface::spatials.end(), this));
}

std::vector<uint8_t> Spatial::move(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(translatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		vec3 moveDelta = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transform = transform * matrix_t(matrix_transform_pt(transform, moveDelta));
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::rotate(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(rotatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		quat rotationDelta = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat(), vector[3].AsFloat() };
		transform = transform * matrix_r(matrix_transform_quat(transform, rotationDelta));
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::scaleThis(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(scalable) {
		float scaleDelta = data.AsFloat();
		transform = transform * matrix_s(matrix_extract_scale(transform) * scaleDelta);
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::getTransform(Client *callingClient, flexbuffers::Reference data, bool) {
	Spatial *space = callingClient->scenegraph.findNode<Spatial>(data.AsString().str());
	if(!space) {
		Alias *spaceAlias = callingClient->scenegraph.findNode<Alias>(data.AsString().str());
		space = spaceAlias ? spaceAlias->original.ptr<Spatial>() : nullptr;
	}
	vec3 pos, scl;
	quat rot;
	matrix_decompose(localToSpaceMatrix(space), pos, scl, rot);

	return StardustXR::FlexbufferFromArguments(
		FLEX_ARGS(
			FLEX_VEC3(pos)
			FLEX_QUAT(rot)
			FLEX_VEC3(scl)
		)
	);
}

std::vector<uint8_t> Spatial::setOrigin(Client *callingClient, flexbuffers::Reference data, bool) {
	if(translatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();

		vec3 pos, scl;
		quat rot;
		matrix_decompose(transform, pos, scl, rot);
		pos = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transform = matrix_trs(pos, rot, scl);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setOrientation(Client *callingClient, flexbuffers::Reference data, bool) {
	if(rotatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();

		vec3 pos, scl;
		quat rot;
		matrix_decompose(transform, pos, scl, rot);
		rot = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat(), vector[3].AsFloat() };
		transform = matrix_trs(pos, rot, scl);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setScale(Client *callingClient, flexbuffers::Reference data, bool) {
	if(scalable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();

		vec3 pos, scl;
		quat rot;
		matrix_decompose(transform, pos, scl, rot);
		scl = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transform = matrix_trs(pos, rot, scl);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setPose(Client *callingClient, flexbuffers::Reference data, bool) {
	if(translatable && rotatable) {
		flexbuffers::Vector vector = data.AsVector();
		setOrigin(callingClient, vector[0], false);
		setOrientation(callingClient, vector[1], false);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setTransform(Client *callingClient, flexbuffers::Reference data, bool) {
	if(translatable && rotatable && scalable) {
		flexbuffers::Vector vector = data.AsVector();
		setOrigin(callingClient, vector[0], false);
		setOrientation(callingClient, vector[1], false);
		setScale(callingClient, vector[2], false);
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::setSpatialParentFlex(Client *callingClient, flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialParent = nullptr;
	} else {
		Spatial *potentialParent = callingClient->scenegraph.findNode<Spatial>(spacePath);
		if(!potentialParent) {
			Alias *potentialParentAlias = callingClient->scenegraph.findNode<Alias>(spacePath);
			if(potentialParentAlias && potentialParentAlias->original)
				potentialParent = potentialParentAlias->original.ptr<Spatial>();
		}
		setSpatialParent(potentialParent);
	}
	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setSpatialParentInPlaceFlex(Client *callingClient, flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialParent = nullptr;
	} else {
		Spatial *potentialParent = callingClient->scenegraph.findNode<Spatial>(spacePath);
		if(!potentialParent) {
			Alias *potentialParentAlias = callingClient->scenegraph.findNode<Alias>(spacePath);
			if(potentialParentAlias)
				potentialParent = potentialParentAlias->original.ptr<Spatial>();
		}
		setSpatialParentInPlace(potentialParent);
	}
	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::setZoneable(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	zoneable = data.AsBool();
	if(!zoneable && this->zone != nullptr)
		this->zone->releaseSpatial(this);
	return std::vector<uint8_t>();
}

matrix Spatial::localToSpaceMatrix(Spatial *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix localToWorldMatrix = worldTransform();

	if(space == nullptr)
		return localToWorldMatrix;

	matrix worldToSpaceMatrix = matrix_invert(space->worldTransform());

	return localToWorldMatrix * worldToSpaceMatrix;
}

matrix Spatial::spaceToLocalMatrix(Spatial *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix worldToLocalMatrix = matrix_invert(worldTransform());

	if(space == nullptr)
		return worldToLocalMatrix;

	matrix spaceToWorldMatrix = space->worldTransform();

	return spaceToWorldMatrix * worldToLocalMatrix;
}

bool Spatial::setSpatialParent(Spatial *spatial) {
	if(spatial == spatialParent)
		return false;

	//Spatial parent loop protection
	Spatial *currentParent = spatial;
	while(currentParent->spatialParent && currentParent->spatialParent != nullptr) {
		if(currentParent->spatialParent->id == this->id)
			return false;
		currentParent = currentParent->spatialParent;
	}

	spatialParent = spatial;
	return true;
}
bool Spatial::setSpatialParentInPlace(Spatial *spatial) {
	if(spatial == spatialParent)
		return false;

	//Spatial parent loop protection
	Spatial *currentParent = spatial;
	while(currentParent->spatialParent && currentParent->spatialParent != nullptr) {
		if(currentParent->spatialParent->id == this->id)
			return false;
		currentParent = currentParent->spatialParent;
	}

	setTransformMatrix(localToSpaceMatrix(spatial));
	spatialParent = spatial;
	return true;
}
void Spatial::setTransformMatrix(matrix mat) {
	transform = mat;
}

matrix Spatial::localTransform() {
	return transform;
}

matrix Spatial::worldTransform() {
	if(spatialParent)
		return localTransform() * spatialParent->worldTransform();
	else
		return localTransform();
}

} // namespace StardustXRServer
