#include "pointer.hpp"
#include <stardustxr/common/flatbuffers/Input.hpp>

namespace StardustXRServer {

PointerInput::PointerInput(Client *client) : InputMethod(client, nullptr, pose_identity, true) {}

PointerInput::~PointerInput() {}

RayMarchResult PointerInput::march(Field *field) {
	ray = {
		vec3_zero,
		vec3_forward,
		this
	};
	return RayMarch(ray, field);
}

DistanceLink PointerInput::makeDistanceLink(InputHandler *handler) {
	RayMarchResult rayInfo = march(handler->field.ptr());
	vec2 compareDistanceTriangle = vec2 {rayInfo.deepestPointDistance, rayInfo.distance};
	return DistanceLink{
		.method = this,
		.compareDistance = vec2_magnitude(compareDistanceTriangle),
		.trueDistance = rayInfo.distance,
		.handler = handler,
	};
}

InputDataRaw PointerInput::type() {
	return InputDataRaw::Pointer;
}

flatbuffers::Offset<void> PointerInput::generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler) {
	sk::vec3 pos = localToSpacePoint(handler, vec3_zero);
	sk::vec3 dir = localToSpaceDirection(handler, vec3_forward);
	float tilt = 0;
	sk::vec3 deepestPoint = pos;

	if(handler->field) {
		RayMarchResult rayInfo = march(handler->field.ptr());
		deepestPoint = dir * rayInfo.deepestPointDistance + pos;
	}

	StardustXR::vec3 flatPos(pos.x, pos.y, pos.z);
	StardustXR::vec3 flatDir(dir.x, dir.y, dir.z);

	StardustXR::vec3 flatDeepestPoint(deepestPoint.x, deepestPoint.y, deepestPoint.z);

	return CreatePointer(fbb, &flatPos, &flatDir, tilt, &flatDeepestPoint).Union();
}

void PointerInput::updateInput(InputData *data, InputHandler *handler) {
	StardustXR::Pointer *pointerInput = static_cast<StardustXR::Pointer *>(data->mutable_input());
	if(!pointerInput)
		return;

	sk::vec3 pos = localToSpacePoint(handler, vec3_zero);
	sk::vec3 dir = localToSpaceDirection(handler, vec3_forward);
	float tilt = 0;

	pointerInput->mutable_origin()->mutate_x(pos.x);
	pointerInput->mutable_origin()->mutate_y(pos.y);
	pointerInput->mutable_origin()->mutate_z(pos.z);

	pointerInput->mutable_direction()->mutate_x(dir.x);
	pointerInput->mutable_direction()->mutate_y(dir.y);
	pointerInput->mutable_direction()->mutate_z(dir.z);

	pointerInput->mutate_tilt(tilt);

	sk::vec3 deepestPoint = pos;
	if(handler->field) {
		RayMarchResult rayInfo = march(handler->field.ptr());
		deepestPoint = dir * rayInfo.deepestPointDistance + pos;
	}

	pointerInput->mutable_deepest_point()->mutate_x(deepestPoint.x);
	pointerInput->mutable_deepest_point()->mutate_y(deepestPoint.y);
	pointerInput->mutable_deepest_point()->mutate_z(deepestPoint.z);
}

void PointerInput::serializeData(flexbuffers::Builder &fbb) {}

} // namespace StardustXRServer
