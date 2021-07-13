#pragma once

#include "node.hpp"
#include <stereokit.h>

using namespace sk;

namespace StardustXRServer {

class SpatialNode : public Node
{
public:
	SpatialNode(Client *client);
	virtual ~SpatialNode() {}

	// Client accessible functions
	std::vector<uint8_t> move                 (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> rotate               (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> rotateAround         (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> scaleThis            (flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> setOrigin            (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setOrientation       (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setScale             (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setPose              (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setTransform         (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSpatialParentFlex (flexbuffers::Reference data, bool returnValue);

	// Surface level parameters
	vec3 position = vec3_zero;
	quat rotation = quat_identity;
	vec3 scale    = vec3_one;

	// Disable parts of this spatialnode (e.g. fields should not be scalable)
	bool translatable = true;
	bool rotatable    = true;
	bool scalable     = true;

	// Matrix get operations
	matrix localTransform();
	matrix worldTransform();
	matrix localToSpaceMatrix(SpatialNode *space);
	matrix spaceToLocalMatrix(SpatialNode *space);

	// Spatial heirarchy relationships
	SpatialNode *spatialParent = nullptr;
	bool setSpatialParent(std::string spacePath);

	vec3 spaceToLocalPoint    (SpatialNode *space, vec3 point    ) { return matrix_mul_point    (spaceToLocalMatrix(space), point    ); }
	vec3 spaceToLocalDirection(SpatialNode *space, vec3 direction) { return matrix_mul_direction(spaceToLocalMatrix(space), direction); }
	quat spaceToLocalRotation (SpatialNode *space, quat rot      ) { return matrix_mul_rotation (spaceToLocalMatrix(space), rot      ); }

	vec3 localToSpacePoint    (SpatialNode *space, vec3 point    ) { return matrix_mul_point    (localToSpaceMatrix(space), point    ); }
	vec3 localToSpaceDirection(SpatialNode *space, vec3 direction) { return matrix_mul_direction(localToSpaceMatrix(space), direction); }
	quat localToSpaceRotation (SpatialNode *space, quat rot      ) { return matrix_mul_rotation (localToSpaceMatrix(space), rot      ); }

	void transformDirty() { transformMatrixDirty = true; }
protected:
	bool transformMatrixDirty = false;
	matrix transform = matrix_identity;
};

} // namespace StardustXRServer
