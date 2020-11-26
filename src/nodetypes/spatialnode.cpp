#include "spatialnode.hpp"

namespace StardustXRServer {

SpatialNode::SpatialNode() {
	STARDUSTXR_NODE_METHOD("setPosition", &SpatialNode::setPosition)
	STARDUSTXR_NODE_METHOD("setRotation", &SpatialNode::setRotation)
	STARDUSTXR_NODE_METHOD("setScale", &SpatialNode::setScale)

	STARDUSTXR_NODE_METHOD("setPose", &SpatialNode::setPose)
	STARDUSTXR_NODE_METHOD("setTransform", &SpatialNode::setTransform)
}

std::vector<uint8_t> SpatialNode::setPosition(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	if(sessionID == this->sessionID) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		position = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transformMatrixDirty = true;
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setRotation(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	if(sessionID == this->sessionID) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		rotation = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat(), vector[3].AsFloat() };
		transformMatrixDirty = true;
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setScale(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	if(sessionID == this->sessionID) {
		flexbuffers::TypedVector vector = data.AsTypedVector();
		scale = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
		transformMatrixDirty = true;
	}

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setPose(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector = data.AsVector();
	setPosition(sessionID, vector[0], false);
	setRotation(sessionID, vector[1], false);
	setScale(sessionID, vector[2], false);

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

std::vector<uint8_t> SpatialNode::setTransform(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector = data.AsVector();
	setPosition(sessionID, vector[0], false);
	setRotation(sessionID, vector[1], false);
	setScale(sessionID, vector[2], false);

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

matrix SpatialNode::localTransform() {
	if(transformMatrixDirty)
		transform = matrix_trs(position, rotation, scale);
	transformMatrixDirty = false;
	return transform;
}

} // namespace StardustXRServer