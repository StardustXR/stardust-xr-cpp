#include "spatial.hpp"
#include "../../globals.h"

#include "../../core/client.hpp"
#include "../../core/scenegraph.hpp"
#include "../../interfaces/spatial.hpp"
#include "../../nodetypes/core/alias.hpp"
#include "stereokit.h"
#include <string>
#include <vector>

using namespace sk;

namespace StardustXRServer {

Registry<Spatial> Spatial::spatials;

Spatial::Spatial(Client *client, matrix transform) : Node(client, false) {
	setTransformMatrix(transform);
}
Spatial::Spatial(Client *client, Spatial *spatialParent, matrix transform, bool translatable, bool rotatable, bool scalable, bool zoneable) :
		Spatial(client, transform) {
	this->spatialParent = spatialParent;
	this->translatable = translatable;
	this->rotatable = rotatable;
	this->scalable = scalable;
	this->zoneable = zoneable;

//	STARDUSTXR_NODE_METHOD("move", &Spatial::move)
//	STARDUSTXR_NODE_METHOD("rotate", &Spatial::rotate)
//	STARDUSTXR_NODE_METHOD("scale", &Spatial::scaleThis)

	STARDUSTXR_NODE_METHOD("getTransform", &Spatial::getTransform)

//	STARDUSTXR_NODE_METHOD("setOrigin", &Spatial::setOrigin)
//	STARDUSTXR_NODE_METHOD("setOrientation", &Spatial::setOrientation)
//	STARDUSTXR_NODE_METHOD("setScale", &Spatial::setScale)

//	STARDUSTXR_NODE_METHOD("setPose", &Spatial::setPose)
	STARDUSTXR_NODE_METHOD("setTransform", &Spatial::setTransform)

	STARDUSTXR_NODE_METHOD("setSpatialParent", &Spatial::setSpatialParentFlex)
	STARDUSTXR_NODE_METHOD("setSpatialParentInPlace", &Spatial::setSpatialParentInPlaceFlex)

	STARDUSTXR_NODE_METHOD("setZoneable", &Spatial::setZoneable)

	spatials.add(this);
}
Spatial::Spatial(Client *client, Spatial *spatialParent, pose_t transform, bool translatable, bool rotatable, bool scalable, bool zoneable) :
	Spatial(client, spatialParent, matrix_trs(transform.position, transform.orientation), translatable, rotatable, scalable, zoneable) {}
Spatial::~Spatial() {
	spatials.remove(this);
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
			if(translatable)
				FLEX_VEC3(pos)
			if(rotatable)
				FLEX_QUAT(rot)
			if(scalable)
				FLEX_VEC3(scl)
		)
	);
}
std::vector<uint8_t> Spatial::setTransform(Client *callingClient, flexbuffers::Reference data, bool) {
	flexbuffers::Vector flexVec = data.AsVector();

	Spatial *space = callingClient->scenegraph.findNode<Spatial>(flexVec[0].AsString().str());
	if(!space) {
		Alias *spaceAlias = callingClient->scenegraph.findNode<Alias>(flexVec[0].AsString().str());
		space = spaceAlias ? spaceAlias->original.ptr<Spatial>() : nullptr;
	}
	if(!space)
		space = this->spatialParent;

	vec3 pos, scl;
	quat rot;
	matrix_decompose(transform, pos, scl, rot);

	if(translatable && flexVec[1].IsTypedVector() && flexVec[1].AsTypedVector().size() == 3) {
		flexbuffers::TypedVector posFlex = flexVec[1].AsTypedVector();
		pos.x = posFlex[0].AsFloat();
		pos.y = posFlex[1].AsFloat();
		pos.z = posFlex[2].AsFloat();
		pos = matrix_transform_pt(spaceToSpaceMatrix(space, this->spatialParent), pos);
	}
	if(rotatable && flexVec[2].IsTypedVector() && flexVec[2].AsTypedVector().size() == 4) {
		flexbuffers::TypedVector rotFlex = flexVec[2].AsTypedVector();
		rot.x = rotFlex[0].AsFloat();
		rot.y = rotFlex[1].AsFloat();
		rot.z = rotFlex[2].AsFloat();
		rot.w = rotFlex[3].AsFloat();
		rot = matrix_transform_quat(spaceToSpaceMatrix(space, this->spatialParent), rot);
	}
	if(scalable && flexVec[3].IsTypedVector() && flexVec[3].AsTypedVector().size() == 3) {
		flexbuffers::TypedVector sclFlex = flexVec[3].AsTypedVector();
		scl.x = sclFlex[0].AsFloat();
		scl.y = sclFlex[1].AsFloat();
		scl.z = sclFlex[2].AsFloat();
	}

	transform = matrix_trs(pos, rot, scl);

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

matrix Spatial::spaceToSpaceMatrix(Spatial *from, Spatial *to) {
	// TODO: Optimize this to check if from and to share a common ancestor
	// and calculate the transform matrix between the two.

	if(from == to)
		return matrix_identity;

	matrix spaceToWorldMatrix = matrix_identity;
	if(from)
		spaceToWorldMatrix = from->worldTransform();

	matrix worldToSpaceMatrix = matrix_identity;
	if(to)
		worldToSpaceMatrix = matrix_invert(to->worldTransform());

	return spaceToWorldMatrix * worldToSpaceMatrix;
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
