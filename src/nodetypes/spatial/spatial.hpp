#pragma once

#include "../core/node.hpp"
#include <stereokit.h>

using namespace sk;

namespace StardustXRServer {

class Spatial : public Node {
public:
	Spatial(Client *client, Spatial *spatialParent, vec3 position, quat rotation, vec3 scale, bool translatable, bool rotatable, bool scalable);
	virtual ~Spatial() {}

	// Client accessible functions
	std::vector<uint8_t> move                        (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> rotate                      (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> scaleThis                   (flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> setOrigin                   (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setOrientation              (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setScale                    (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setPose                     (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setTransform                (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSpatialParentFlex        (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSpatialParentInPlaceFlex (flexbuffers::Reference data, bool returnValue);

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
	matrix localToSpaceMatrix(Spatial *space);
	matrix spaceToLocalMatrix(Spatial *space);

	// Spatial heirarchy relationships
	Spatial *spatialParent = nullptr;
	bool setSpatialParent(std::string spacePath);
	bool setSpatialParentInPlace(std::string spacePath);

	vec3 spaceToLocalPoint    (Spatial *space, vec3 point    ) { return matrix_transform_pt  (spaceToLocalMatrix(space), point    ); }
	vec3 spaceToLocalDirection(Spatial *space, vec3 direction) { return matrix_transform_dir (spaceToLocalMatrix(space), direction); }
	quat spaceToLocalRotation (Spatial *space, quat rot      ) { return matrix_transform_quat(spaceToLocalMatrix(space), rot      ); }

	vec3 localToSpacePoint    (Spatial *space, vec3 point    ) { return matrix_transform_pt  (localToSpaceMatrix(space), point    ); }
	vec3 localToSpaceDirection(Spatial *space, vec3 direction) { return matrix_transform_dir (localToSpaceMatrix(space), direction); }
	quat localToSpaceRotation (Spatial *space, quat rot      ) { return matrix_transform_quat(localToSpaceMatrix(space), rot      ); }

	void transformDirty() { transformMatrixDirty = true; }
protected:
	bool transformMatrixDirty = false;
	matrix transform = matrix_identity;
};

} // namespace StardustXRServer
