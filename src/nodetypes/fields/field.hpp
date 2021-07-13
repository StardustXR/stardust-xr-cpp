#pragma once

#include "../spatialnode.hpp"

namespace StardustXRServer {

class Field : public SpatialNode {
public:
	Field(Client *client);
	virtual ~Field() {}

	float normalRadius = 0.0000001f;

	// Stardust XR Server specific methods, overriding them is only for optimization
	virtual       float distance     (SpatialNode *space, const vec3 point);
	virtual const vec3  closestPoint (SpatialNode *space, const vec3 point);
	virtual const vec3  normal       (SpatialNode *space, const vec3 point);
	virtual const vec3  normal       (SpatialNode *space, const vec3 point, const float radius);

	// Internal field methods, overriding localDistance and/or localClosestPoint is required in all subclasses
	virtual       float localDistance     (const vec3 point);
	virtual const vec3  localClosestPoint (const vec3 point);
	virtual const vec3  localNormal       (const vec3 point);
	virtual const vec3  localNormal       (const vec3 point, const float radius);

	// Client-facing methods
	std::vector<uint8_t> distance(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> normal(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> closestPoint(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
