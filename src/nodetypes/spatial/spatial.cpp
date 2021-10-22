#include "spatial.hpp"
#include "../../globals.h"

#include "../../core/client.hpp"
#include "../../core/scenegraph.hpp"
#include "../../interfaces/spatial.hpp"
#include "stereokit.h"
#include <string>
#include <vector>
using namespace StardustXR;
namespace StardustXRServer {

Spatial::Spatial(Client *client, Spatial *spatialParent, vec3 position, quat rotation, vec3 scale, bool translatable, bool rotatable, bool scalable, bool zoneable) : Node(client) {
	this->spatialParent = spatialParent;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->translatable = translatable;
	this->rotatable = rotatable;
	this->scalable = scalable;
	this->zoneable = zoneable;
	this->transformDirty();

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
Spatial::~Spatial() {
	std::lock_guard<std::mutex> lock(SpatialInterface::spatialMutex);
	SpatialInterface::spatials.erase(std::remove(SpatialInterface::spatials.begin(), SpatialInterface::spatials.end(), this));
}

std::vector<uint8_t> Spatial::move(flexbuffers::Reference data, bool returnValue) {
	if(translatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		vec3 moveDelta = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		position += rotation * moveDelta;
		transformMatrixDirty = true;
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::rotate(flexbuffers::Reference data, bool returnValue) {
	if(rotatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		quat rotationDelta = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat(), vector[3].AsFloat() };
		rotation = rotation * rotationDelta;
		transformMatrixDirty = true;
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::scaleThis(flexbuffers::Reference data, bool returnValue) {
	if(translatable) {
		float scaleDelta = data.AsFloat();
		scale = scale * scaleDelta;
		transformMatrixDirty = true;
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::getTransform(flexbuffers::Reference data, bool) {
	vec3 pos = vec3_zero;
	return StardustXR::FlexbufferFromArguments(
		FLEX_ARGS(
			FLEX_VEC3(position)
			FLEX_QUAT(rotation)
			FLEX_VEC3(scale)
		)
	);
}

std::vector<uint8_t> Spatial::setOrigin(flexbuffers::Reference data, bool) {
	if(translatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		position = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transformMatrixDirty = true;
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setOrientation(flexbuffers::Reference data, bool) {
	if(rotatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		rotation = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat(), vector[3].AsFloat() };
		transformMatrixDirty = true;
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setScale(flexbuffers::Reference data, bool) {
	if(scalable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		scale = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transformMatrixDirty = true;
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setPose(flexbuffers::Reference data, bool) {
	if(translatable && rotatable) {
		flexbuffers::Vector vector = data.AsVector();
		setOrigin(vector[0], false);
		setOrientation(vector[1], false);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setTransform(flexbuffers::Reference data, bool) {
	if(translatable && rotatable && scalable) {
		flexbuffers::Vector vector = data.AsVector();
		setOrigin(vector[0], false);
		setOrientation(vector[1], false);
		setScale(vector[2], false);
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::setSpatialParentFlex(flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialParent = nullptr;
	} else {
		Spatial *potentialParent = client->scenegraph.findNode<Spatial>(spacePath);
		setSpatialParent(potentialParent);
	}
	return std::vector<uint8_t>();
}
std::vector<uint8_t> Spatial::setSpatialParentInPlaceFlex(flexbuffers::Reference data, bool) {
	std::string spacePath = data.AsString().str();
	if(spacePath == "") {
		spatialParent = nullptr;
	} else {
		Spatial *potentialParent = client->scenegraph.findNode<Spatial>(spacePath);
		setSpatialParentInPlace(potentialParent);
	}
	return std::vector<uint8_t>();
}

std::vector<uint8_t> Spatial::setZoneable(flexbuffers::Reference data, bool returnValue) {
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

	spatialParent = spatial;
	return true;
}
bool Spatial::setSpatialParentInPlace(Spatial *spatial) {
	if(spatial == spatialParent)
		return false;

	transform = localToSpaceMatrix(spatial);
	matrix_decompose(transform, position, scale, rotation);

	spatialParent = spatial;
	return true;
}

matrix Spatial::localTransform() {
	if(transformMatrixDirty)
		transform = matrix_trs(position, rotation, scale);
	transformMatrixDirty = false;
	return transform;
}

matrix Spatial::worldTransform() {
	if(spatialParent)
		return localTransform() * spatialParent->worldTransform();
	else
		return localTransform();
}


} // namespace StardustXRServer
