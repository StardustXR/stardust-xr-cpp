#pragma once

#include "../spatial/spatial.hpp"

namespace StardustXRServer {

class Field : public Spatial {
public:
	Field(Client *client, Spatial *spatialParent, sk::pose_t transform, bool rotatable);
	virtual ~Field() {}

	float normalRadius = 0.0000001f;

	// Stardust XR Server specific methods, overriding them is only for optimization
	virtual       float    distance     (Spatial *space, const sk::vec3 point);
	virtual const sk::vec3 closestPoint (Spatial *space, const sk::vec3 point);
	virtual const sk::vec3 normal       (Spatial *space, const sk::vec3 point);
	virtual const sk::vec3 normal       (Spatial *space, const sk::vec3 point, const float radius);

	// Internal field methods, overriding localDistance and/or localClosestPoint is required in all subclasses
	virtual       float    localDistance     (const sk::vec3 point);
	virtual const sk::vec3 localClosestPoint (const sk::vec3 point);
	virtual const sk::vec3 localNormal       (const sk::vec3 point);
	virtual const sk::vec3 localNormal       (const sk::vec3 point, const float radius);

	// Client-facing methods
	std::vector<uint8_t> distance(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> normal(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> closestPoint(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
