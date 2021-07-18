#include "spatial.hpp"
#include "../globals.h"

#include "../core/client.hpp"
#include "../core/scenegraph.hpp"
#include <vector>
using namespace StardustXR;
namespace StardustXRServer {

Spatial::Spatial(Client *client) : Node(client) {
	STARDUSTXR_NODE_METHOD("move", &Spatial::move)
	STARDUSTXR_NODE_METHOD("rotate", &Spatial::rotate)
	STARDUSTXR_NODE_METHOD("rotateAround", &Spatial::rotateAround)
	STARDUSTXR_NODE_METHOD("scale", &Spatial::scaleThis)

	STARDUSTXR_NODE_METHOD("setOrigin", &Spatial::setOrigin)
	STARDUSTXR_NODE_METHOD("setOrientation", &Spatial::setOrientation)
	STARDUSTXR_NODE_METHOD("setScale", &Spatial::setScale)

	STARDUSTXR_NODE_METHOD("setPose", &Spatial::setPose)
	STARDUSTXR_NODE_METHOD("setTransform", &Spatial::setTransform)

	STARDUSTXR_NODE_METHOD("setSpatialParent", &Spatial::setSpatialParentFlex)
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

std::vector<uint8_t> Spatial::rotateAround(flexbuffers::Reference data, bool returnValue) {
	if(rotatable) {
		flexbuffers::Vector vec = data.AsVector();
		flexbuffers::TypedVector flexPoint = vec[0].AsTypedVector();
		flexbuffers::TypedVector flexRotation = vec[0].AsTypedVector();

		vec3 point = { flexPoint[0].AsFloat(), flexPoint[1].AsFloat(), flexPoint[2].AsFloat() };
		quat rotationDelta = { flexRotation[0].AsFloat(), flexRotation[1].AsFloat(), flexRotation[2].AsFloat(), flexRotation[3].AsFloat() };

		position += rotation * point;
		rotation = rotation * rotationDelta;
		position -= rotation * point;

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
		setScale(vector[2], false);
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
	setSpatialParent(spacePath);
	return std::vector<uint8_t>();
}

matrix Spatial::localToSpaceMatrix(Spatial *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix localToWorldMatrix = worldTransform();

	if(space == nullptr || space->ready == false)
		return localToWorldMatrix;

	matrix worldToSpaceMatrix;
	matrix_inverse(space->worldTransform(), worldToSpaceMatrix);

	return localToWorldMatrix * worldToSpaceMatrix;
}

matrix Spatial::spaceToLocalMatrix(Spatial *space) {
	// TODO: Optimize this to check if space and this SpatialNode share a common ancestor
	// and calculate the transform matrix between the two.

	matrix worldToLocalMatrix;
	matrix_inverse(worldTransform(), worldToLocalMatrix);

	if(space == nullptr || space->ready == false)
		return worldToLocalMatrix;

	matrix spaceToWorldMatrix = space->worldTransform();

	return spaceToWorldMatrix * worldToLocalMatrix;
}

bool Spatial::setSpatialParent(std::string spacePath) {
	if(spacePath == "") {
		spatialParent = nullptr;
		return true;
	}
	Spatial *potentialParent = dynamic_cast<Spatial *>(client->scenegraph.findNode(spacePath));
	if(potentialParent) {
		spatialParent = potentialParent;
		return true;
	}
	return false;
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
