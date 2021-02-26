#include "flatscreenpointer.hpp"

namespace StardustXRServer {

FlatscreenPointer::FlatscreenPointer() {
	datamap["select"] = 0.0f;
	datamap["grab"] = 0.0f;
	datamap["scroll"] = vec2{0, 0};
}

void FlatscreenPointer::update() {
	position = input_head()->position;
	rotation = quat_lookat(input_head()->position, input_hand(handed_right)->fingers[1][4].position);
	datamap["select"] = ((input_hand(handed_right)->pinch_state & button_state_active) != 0) ? 1.0f : 0.0f;
}

} // namespace StardustXRServer
