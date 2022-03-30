#include "flatscreenpointer.hpp"
#include "systems/input.h"

namespace StardustXRServer {

FlatscreenPointer::FlatscreenPointer(Client *client) : PointerInput(client) {}

void FlatscreenPointer::update() {
	ray_t mouseRay;
	if(ray_from_mouse(input_mouse()->pos, mouseRay)) {
		transform = matrix_trs(mouseRay.pos, quat_lookat(vec3_zero, mouseRay.dir));
	}
}

void FlatscreenPointer::serializeData(flexbuffers::Builder &fbb) {
	fbb.String("type", "mouse");

	bool left = input_key(sk::key_mouse_left) & button_state_active;
	bool middle = input_key(sk::key_mouse_center) & button_state_active;
	bool right = input_key(sk::key_mouse_right) & button_state_active;

	fbb.Bool("left", left);
	fbb.Bool("middle", middle);
	fbb.Bool("right", right);
	fbb.TypedVector("scroll", [&]() {
		fbb.Float(0);
		fbb.Float(input_mouse()->scroll_change / 120.0f);
	});

	fbb.Bool("select", left);
	fbb.Bool("rotate", middle);
	fbb.Bool("context", right);
}

} // namespace StardustXRServer
