#pragma once

#include "../spatial/spatial.hpp"

namespace StardustXRServer {

class Field : public Spatial {
public:
	Field(Client *client, Spatial *spatialParent, vec3 position, quat rotation, bool rotatable);
	virtual ~Field() {}

	float normalRadius = 0.0000001f;

	// Stardust XR Server specific methods, overriding them is only for optimization
	virtual       float distance     (Spatial *space, const vec3 point);
	virtual const vec3  closestPoint (Spatial *space, const vec3 point);
	virtual const vec3  normal       (Spatial *space, const vec3 point);
	virtual const vec3  normal       (Spatial *space, const vec3 point, const float radius);

	// Internal field methods, overriding localDistance and/or localClosestPoint is required in all subclasses
	virtual       float localDistance     (const vec3 point);
	virtual const vec3  localClosestPoint (const vec3 point);
	virtual const vec3  localNormal       (const vec3 point);
	virtual const vec3  localNormal       (const vec3 point, const float radius);

	// Client-facing methods
	std::vector<uint8_t> distance(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> normal(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> closestPoint(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
