#pragma once

#include "../core/node.hpp"
#include "stereokit.h"

namespace StardustXRServer {

class Zone;

class Spatial : public Node {
public:
	Spatial(Client *client, sk::matrix transform);
	Spatial(Client *client, Spatial *spatialParent, sk::pose_t transform, bool translatable, bool rotatable, bool scalable, bool zoneable);
	Spatial(Client *client, Spatial *spatialParent, sk::matrix transform, bool translatable, bool rotatable, bool scalable, bool zoneable);
	static Registry<Spatial> spatials;
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
	static sk::matrix spaceToSpaceMatrix(Spatial *from, Spatial *to);
	sk::matrix localTransform();
	sk::matrix worldTransform();
	sk::matrix localToSpaceMatrix(Spatial *space);
	sk::matrix spaceToLocalMatrix(Spatial *space);

	// Spatial heirarchy relationships
	Zone *zone = nullptr;
	Spatial *spatialParent = nullptr;
	Spatial *originalSpatialParent = nullptr;
	bool setSpatialParent(Spatial *spatial);
	bool setSpatialParentInPlace(Spatial *spatial);
	void setTransformMatrix(sk::matrix mat);

	sk::vec3 spaceToLocalPoint    (Spatial *space, sk::vec3 point    ) { return matrix_transform_pt  (spaceToLocalMatrix(space), point    ); }
	sk::vec3 spaceToLocalDirection(Spatial *space, sk::vec3 direction) { return matrix_transform_dir (spaceToLocalMatrix(space), direction); }
	sk::quat spaceToLocalRotation (Spatial *space, sk::quat rot      ) { return matrix_transform_quat(spaceToLocalMatrix(space), rot      ); }

	sk::vec3 localToSpacePoint    (Spatial *space, sk::vec3 point    ) { return matrix_transform_pt  (localToSpaceMatrix(space), point    ); }
	sk::vec3 localToSpaceDirection(Spatial *space, sk::vec3 direction) { return matrix_transform_dir (localToSpaceMatrix(space), direction); }
	sk::quat localToSpaceRotation (Spatial *space, sk::quat rot      ) { return matrix_transform_quat(localToSpaceMatrix(space), rot      ); }
protected:
	sk::matrix transform = sk::matrix_identity;
};

} // namespace StardustXRServer
