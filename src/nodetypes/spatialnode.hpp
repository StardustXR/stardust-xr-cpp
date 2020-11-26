#ifndef STARDUSTXRSERVER_SPATIALNODE_HPP
#define STARDUSTXRSERVER_SPATIALNODE_HPP

#include "node.hpp"
#include <stereokit.h>

using namespace sk;

namespace StardustXRServer {

class SpatialNode : public Node
{
public:
	SpatialNode();
	virtual ~SpatialNode() {}

	// Client accessible functions
	std::vector<uint8_t> setPosition (uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setRotation (uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setScale    (uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setPose     (uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setTransform(uint sessionID, flexbuffers::Reference data, bool returnValue);

	// Surface level parameters
	vec3 position = vec3_zero;
	quat rotation = quat_identity;
	vec3 scale    = vec3_one;

	// Matrix get operations
	matrix localTransform();
	matrix worldTransform();
	matrix localToSpaceMatrix(SpatialNode *space);
	matrix spaceToLocalMatrix(SpatialNode *space);

	// Spatial heirarchy relationships
	SpatialNode *spatialParent = nullptr;

	void transformDirty() { transformMatrixDirty = true; }
protected:
	bool transformMatrixDirty = false;
	matrix transform = matrix_identity;
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_SPATIALNODE_HPP
