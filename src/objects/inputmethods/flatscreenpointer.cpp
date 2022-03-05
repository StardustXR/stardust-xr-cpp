#include "flatscreenpointer.hpp"
#include "systems/input.h"

namespace StardustXRServer {

FlatscreenPointer::FlatscreenPointer(Client *client) : PointerInput(client) {
	datamap["select"] = 0.0f;
	datamap["grab"] = 0.0f;
	datamap["scroll"] = vec2{0, 0};

	datamap["type"] = "mouse";
}

void FlatscreenPointer::update() {
	ray_t mouseRay;
	if(ray_from_mouse(input_mouse()->pos, mouseRay)) {
		transform = matrix_trs(mouseRay.pos, quat_lookat(vec3_zero, mouseRay.dir));
//		transformDirty();
		datamap["left"]   = (input_key(sk::key_mouse_left)   & button_state_active) ? 1.0f : 0.0f;
		datamap["middle"] = (input_key(sk::key_mouse_center) & button_state_active) ? 1.0f : 0.0f;
		datamap["right"]  = (input_key(sk::key_mouse_right)  & button_state_active) ? 1.0f : 0.0f;

		datamap["select"] = datamap["left"];
		datamap["rotate"] = datamap["middle"];
		datamap["context"] = datamap["right"];

		datamap["scroll"] = vec2{0, input_mouse()->scroll_change / 120.0f};
	}
}

} // namespace StardustXRServer
