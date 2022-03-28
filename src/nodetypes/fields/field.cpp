#include "field.hpp"
#include <stardustxr/common/flex.hpp>
#include "../../core/client.hpp"
#include <stereokit.h>

using namespace StardustXR;
namespace StardustXRServer {

Field::Field(Client *client, Spatial *spatialParent, pose_t transform, bool rotatable) :
Spatial(client, spatialParent, transform, true, rotatable, false, false) {
	STARDUSTXR_NODE_METHOD("distance",     static_cast<std::vector<uint8_t>(Field::*)(Client *, flexbuffers::Reference, bool)>(&Field::distance))
	STARDUSTXR_NODE_METHOD("normal",       static_cast<std::vector<uint8_t>(Field::*)(Client *, flexbuffers::Reference, bool)>(&Field::normal))
	STARDUSTXR_NODE_METHOD("closestPoint", static_cast<std::vector<uint8_t>(Field::*)(Client *, flexbuffers::Reference, bool)>(&Field::closestPoint))
}

float Field::localDistance(const vec3 point) {
	return vec3_magnitude(point - localClosestPoint(point));
}

const vec3 Field::localClosestPoint(const vec3 point) {
	return point - (localNormal(point) * localDistance(point));
}

const vec3 Field::localNormal(const vec3 point) {
	return localNormal(point, normalRadius);
}

const vec3 Field::localNormal(const vec3 point, float radius) {
	float d = localDistance(point);
	vec2 e = { radius, 0.0f };

	vec3 n = vec3{ d,d,d } - vec3{
		localDistance(vec3{e.x, e.y, e.y}),
		localDistance(vec3{e.y, e.x, e.y}),
		localDistance(vec3{e.y, e.y, e.x})
	};

	return vec3_normalize(n);
}

float Field::distance(Spatial *space, const vec3 point) {
	vec3 localPoint = spaceToLocalPoint(space, point);

	vec3 scale = matrix_extract_scale(worldTransform());

	if(scale.x == 1.0f && scale.x == scale.y && scale.y == scale.z) { // We should really fix this but idk how??
		//If the field is not scaled
		return localDistance(localPoint);
	} else if(scale.x == scale.y && scale.y == scale.z) {
		//If the field is scaled uniformly
		float scaleFactor = scale.x;
		return localDistance(localPoint*scaleFactor);
	} else {
		//If the field is scaled non-uniformly. Should be avoided if possible due to inefficiency.
		vec3 localSurfacePoint = localClosestPoint(localPoint);
		float localScaledDistance = localDistance(localPoint); //Get the distance so we know if the point is inside or outside

		vec3 surfacePoint = localToSpacePoint(space, localSurfacePoint);
		float distance = vec3_magnitude(point - surfacePoint);
		return copysignf(distance, localScaledDistance); //Apply the sign of the DE to tell if the point is inside/outside
	}
}

const vec3 Field::closestPoint(Spatial *space, const vec3 point) {
	vec3 localPoint = spaceToLocalPoint(space, point);
	return localToSpaceDirection(space, localClosestPoint(localPoint));
}

const vec3 Field::normal(Spatial *space, const vec3 point) {
	return normal(space, point, normalRadius);
}

const vec3 Field::normal(Spatial *space, const vec3 point, float radius) {
	vec3 localPoint = spaceToLocalPoint(space, point);
	return localToSpaceDirection(space, localNormal(localPoint, radius));
}

std::vector<uint8_t> Field::distance(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!returnValue) // Don't do anything if the function doesn't return a value or you don't have access to it (why would you even run this without expecting something back??)
		return std::vector<uint8_t>();

	flexbuffers::Vector vec = data.AsVector();
	std::string spacePath = vec[0].AsString().str();
	flexbuffers::TypedVector flexPoint = vec[1].AsTypedVector();
	vec3 point = {
		flexPoint[0].AsFloat(),
		flexPoint[1].AsFloat(),
		flexPoint[2].AsFloat()
	};

	//If the spacePath doesn't exist, it must be world space
	float distance = this->distance(callingClient->scenegraph.findNode<Spatial>(spacePath), point);

	return FLEX_SINGLE(
		FLEX_FLOAT(distance)
	);
}

std::vector<uint8_t> Field::normal(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!returnValue) // Don't do anything if the function doesn't return a value or you don't have access to it (why would you even run this without expecting something back??)
		return std::vector<uint8_t>();

	flexbuffers::Vector vec = data.AsVector();
	std::string spacePath = vec[0].AsString().str();
	flexbuffers::TypedVector flexPoint = vec[1].AsTypedVector();
	vec3 point = {
		flexPoint[0].AsFloat(),
		flexPoint[1].AsFloat(),
		flexPoint[2].AsFloat()
	};

	//If the spacePath doesn't exist, it must be world space
	vec3 normal = this->normal(callingClient->scenegraph.findNode<Spatial>(spacePath), point);

	return FLEX_SINGLE(
		FLEX_VEC3(normal)
	);
}

std::vector<uint8_t> Field::closestPoint(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!returnValue) // Don't do anything if the function doesn't return a value or you don't have access to it (why would you even run this without expecting something back??)
		return std::vector<uint8_t>();

	flexbuffers::Vector vec = data.AsVector();
	std::string spacePath = vec[0].AsString().str();
	flexbuffers::TypedVector flexPoint = vec[1].AsTypedVector();
	vec3 point = {
		flexPoint[0].AsFloat(),
		flexPoint[1].AsFloat(),
		flexPoint[2].AsFloat()
	};

	//If the spacePath doesn't exist, it must be world space
	vec3 closestPoint = this->normal(callingClient->scenegraph.findNode<Spatial>(spacePath), point);

	return FLEX_SINGLE(
		FLEX_VEC3(closestPoint)
	);
}

} // namespace StardustXRServer
