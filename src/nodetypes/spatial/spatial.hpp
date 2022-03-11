#pragma once

#include "../core/node.hpp"
#include <stereokit.h>

using namespace sk;

namespace StardustXRServer {

class Zone;

class Spatial : public Node {
public:
	Spatial(Client *client, matrix transformMatrix);
	Spatial(Client *client, Spatial *spatialParent, matrix transformMatrix, bool translatable, bool rotatable, bool scalable, bool zoneable);
	Spatial(Client *client, Spatial *spatialParent, vec3 position, quat rotation, vec3 scale, bool translatable, bool rotatable, bool scalable, bool zoneable);
	~Spatial();

	std::vector<uint8_t> getTransform                (Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setTransform                (Client *callingClient, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> setSpatialParentFlex        (Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSpatialParentInPlaceFlex (Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setZoneable                 (Client *callingClient, flexbuffers::Reference data, bool returnValue);

	// Disable parts of this spatialnode (e.g. fields should not be scalable)
	bool translatable = true;
	bool rotatable    = true;
	bool scalable     = true;
	bool zoneable     = false;

	// Matrix get operations
	static matrix spaceToSpaceMatrix(Spatial *from, Spatial *to);
	matrix localTransform();
	matrix worldTransform();
	matrix localToSpaceMatrix(Spatial *space);
	matrix spaceToLocalMatrix(Spatial *space);

	// Spatial heirarchy relationships
	Zone *zone = nullptr;
	Spatial *spatialParent = nullptr;
	Spatial *originalSpatialParent = nullptr;
	bool setSpatialParent(Spatial *spatial);
	bool setSpatialParentInPlace(Spatial *spatial);
	void setTransformMatrix(matrix mat);

	vec3 spaceToLocalPoint    (Spatial *space, vec3 point    ) { return matrix_transform_pt  (spaceToLocalMatrix(space), point    ); }
	vec3 spaceToLocalDirection(Spatial *space, vec3 direction) { return matrix_transform_dir (spaceToLocalMatrix(space), direction); }
	quat spaceToLocalRotation (Spatial *space, quat rot      ) { return matrix_transform_quat(spaceToLocalMatrix(space), rot      ); }

	vec3 localToSpacePoint    (Spatial *space, vec3 point    ) { return matrix_transform_pt  (localToSpaceMatrix(space), point    ); }
	vec3 localToSpaceDirection(Spatial *space, vec3 direction) { return matrix_transform_dir (localToSpaceMatrix(space), direction); }
	quat localToSpaceRotation (Spatial *space, quat rot      ) { return matrix_transform_quat(localToSpaceMatrix(space), rot      ); }
protected:
	matrix transform = matrix_identity;
};

} // namespace StardustXRServer
