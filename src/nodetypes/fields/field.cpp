#include "field.hpp"

namespace StardustXRServer {

Field::Field() {

}

const float Field::localDistance(const vec3 point) {
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

const float Field::distance(const vec3 point) {
	cleanTransform();
	matrix inverse_transform_matrix;
	matrix_inverse(transform, inverse_transform_matrix);
	vec3 localPoint = matrix_mul_point(inverse_transform_matrix, point);

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

		vec3 surfacePoint = matrix_mul_point(transform, localSurfacePoint);
		float distance = vec3_magnitude(point - surfacePoint);
		return copysignf(distance, localScaledDistance); //Apply the sign of the DE to tell if the point is inside/outside
	}
}

const vec3 Field::closestPoint(const vec3 point) {
	cleanTransform();
	matrix inverse_transform_matrix;
	matrix_inverse(transform, inverse_transform_matrix);
	vec3 localPoint = matrix_mul_point(inverse_transform_matrix, point);
	return matrix_mul_point(transform, localClosestPoint(localPoint));
}

const vec3 Field::normal(const vec3 point) {
	return normal(point, normalRadius);
}

const vec3 Field::normal(const vec3 point, float radius) {
	matrix inverse_transform_matrix;
	matrix_inverse(transform, inverse_transform_matrix);
	vec3 localPoint = matrix_mul_point(inverse_transform_matrix, point);
	return matrix_mul_direction(transform, localNormal(localPoint, radius));
}

} // namespace StardustXRServer
