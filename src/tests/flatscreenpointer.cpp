#include "flatscreenpointer.hpp"

namespace StardustXRServer {

FlatscreenPointer::FlatscreenPointer(Client *client) : PointerInput(client) {
	datamap["select"] = 0.0f;
	datamap["grab"] = 0.0f;
	datamap["scroll"] = vec2{0, 0};
}

void FlatscreenPointer::update() {
	ray_t mouseRay;
	if(ray_from_mouse(input_mouse()->pos, mouseRay)) {
		position = mouseRay.pos;
		rotation = quat_lookat(vec3_zero, mouseRay.dir);
		transformDirty();
		datamap["select"] = ((input_hand(handed_right)->pinch_state & button_state_active) != 0) ? 1.0f : 0.0f;
		datamap["scroll"] = vec2{0, input_mouse()->scroll_change};
	}
}

} // namespace StardustXRServer
