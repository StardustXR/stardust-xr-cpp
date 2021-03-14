#include "spatialnode.hpp"
#include "../globals.h"

namespace StardustXRServer {

SpatialNode::SpatialNode() {
	STARDUSTXR_NODE_METHOD("setPosition", &SpatialNode::setPosition)
	STARDUSTXR_NODE_METHOD("setRotation", &SpatialNode::setRotation)
	STARDUSTXR_NODE_METHOD("setScale", &SpatialNode::setScale)

	STARDUSTXR_NODE_METHOD("setPose", &SpatialNode::setPose)
	STARDUSTXR_NODE_METHOD("setTransform", &SpatialNode::setTransform)
}

std::vector<uint8_t> SpatialNode::setPosition(uint sessionID, flexbuffers::Reference data, bool) {
	if(sessionID == this->sessionID && translatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		position = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transformMatrixDirty = true;
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setRotation(uint sessionID, flexbuffers::Reference data, bool) {
	if(sessionID == this->sessionID && rotatable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		rotation = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat(), vector[3].AsFloat() };
		transformMatrixDirty = true;
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setScale(uint sessionID, flexbuffers::Reference data, bool) {
	if(sessionID == this->sessionID && scalable) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		scale = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transformMatrixDirty = true;
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setPose(uint sessionID, flexbuffers::Reference data, bool) {
	if(sessionID == this->sessionID && translatable && rotatable) {
		flexbuffers::Vector vector = data.AsVector();
		setPosition(sessionID, vector[0], false);
		setRotation(sessionID, vector[1], false);
		setScale(sessionID, vector[2], false);
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setTransform(uint sessionID, flexbuffers::Reference data, bool) {
	if(sessionID == this->sessionID && translatable && rotatable && scalable) {
		flexbuffers::Vector vector = data.AsVector();
		setPosition(sessionID, vector[0], false);
		setRotation(sessionID, vector[1], false);
		setScale(sessionID, vector[2], false);
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

matrix SpatialNode::localToSpaceMatrix(SpatialNode *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix localToWorldMatrix = worldTransform();

	if(space == nullptr || space->ready == false)
		return localToWorldMatrix;

	matrix worldToSpaceMatrix;
	matrix_inverse(space->worldTransform(), worldToSpaceMatrix);

	return localToWorldMatrix * worldToSpaceMatrix;
}

matrix SpatialNode::spaceToLocalMatrix(SpatialNode *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix worldToLocalMatrix;
	matrix_inverse(worldTransform(), worldToLocalMatrix);

	if(space == nullptr || space->ready == false)
		return worldToLocalMatrix;

	matrix spaceToWorldMatrix = space->worldTransform();

	return spaceToWorldMatrix * worldToLocalMatrix;
}

bool SpatialNode::setSpatialParent(std::string spacePath) {
	if(spacePath == "") {
		spatialParent = nullptr;
		return true;
	}
	SpatialNode *potentialParent = dynamic_cast<SpatialNode *>(scenegraph.findNode(spacePath));
	if(potentialParent) {
		spatialParent = potentialParent;
		return true;
	}
	return false;
}

matrix SpatialNode::localTransform() {
	if(transformMatrixDirty)
		transform = matrix_trs(position, rotation, scale);
	transformMatrixDirty = false;
	return transform;
}

matrix SpatialNode::worldTransform() {
	if(spatialParent)
		return localTransform() * spatialParent->worldTransform();
	else
		return localTransform();
}


} // namespace StardustXRServer
