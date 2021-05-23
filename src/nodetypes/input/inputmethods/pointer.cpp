#include "pointer.hpp"
#include "flatbuffers/PointerInput_generated.h"

namespace StardustXRServer {

PointerInput::PointerInput() {
	translatable = true;
	rotatable = true;
	scalable = false;
}

PointerInput::~PointerInput() {

}

float PointerInput::distanceTo(InputHandler *handler) {
	ray = {
		vec3_zero,
		vec3_forward,
		this
	};
	RayMarchResult rayInfo = RayMarch(ray, handler->field);

	datamap["deepestPointDistance"] = sk::vec3_magnitude(rayInfo.deepestPoint);

	return rayInfo.distance;
}

InputDataRaw PointerInput::type() {
	return InputDataRaw_Pointer;
}

flatbuffers::Offset<void> PointerInput::generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler) {
	sk::vec3 pos = localToSpacePoint(handler, vec3_zero);
	sk::vec3 dir = localToSpaceDirection(handler, vec3_forward);
	float tilt = 0;

	StardustXR::vec3 flatPos(pos.x, pos.y, pos.z);
	StardustXR::vec3 flatDir(dir.x, dir.y, dir.z);

	return CreatePointer(fbb, &flatPos, &flatDir, tilt).Union();
}

void PointerInput::updateInput(InputData *data, SpatialNode *space) {
	StardustXR::Pointer *pointerInput = static_cast<StardustXR::Pointer *>(data->mutable_input());
	if(!pointerInput)
		return;

	sk::vec3 pos = localToSpacePoint(space, vec3_zero);
	sk::vec3 dir = localToSpaceDirection(space, vec3_forward);
	float tilt = 0;

	pointerInput->mutable_origin()->mutate_x(pos.x);
	pointerInput->mutable_origin()->mutate_y(pos.y);
	pointerInput->mutable_origin()->mutate_z(pos.z);

	pointerInput->mutable_direction()->mutate_x(dir.x);
	pointerInput->mutable_direction()->mutate_y(dir.y);
	pointerInput->mutable_direction()->mutate_z(dir.z);

	pointerInput->mutate_tilt(tilt);
}

} // namespace StardustXRServer
