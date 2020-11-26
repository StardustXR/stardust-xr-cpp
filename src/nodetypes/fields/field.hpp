#ifndef STARDUSTXRSERVER_FIELD_HPP
#define STARDUSTXRSERVER_FIELD_HPP

#include "../spatialnode.hpp"

namespace StardustXRServer {

class Field : public SpatialNode {
public:
	Field();
	virtual ~Field();

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
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_FIELD_HPP
