#include "flatscreenpointer.hpp"
#include "systems/input.h"

#include "keymap.hpp"

#include <xkbcommon/xkbcommon.h>

namespace StardustXRServer {

FlatscreenPointer::FlatscreenPointer(Client *client, float keySendDistance) :
PointerInput(client),
keySendDistance(keySendDistance),
keyMask(FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
	fbb.Map([&fbb]() {
		fbb.String("type", "keyboard");
	});
})),
keySender(client, this, pose_identity) {
	xkb_context *ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	xkb_keymap *kmp = xkb_keymap_new_from_string(ctx, skKeymapString.c_str(), XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	shiftIndex = xkb_keymap_mod_get_index(kmp, "Shift");
	capsIndex =  xkb_keymap_mod_get_index(kmp, "Lock");
	ctrlIndex =  xkb_keymap_mod_get_index(kmp, "Control");
	altIndex =   xkb_keymap_mod_get_index(kmp, "Mod4");
	xkb_keymap_unref(kmp);
	xkb_context_unref(ctx);
}

void FlatscreenPointer::update() {
	ray_t mouseRay;
	if(ray_from_mouse(input_mouse()->pos, mouseRay)) {
		transform = matrix_trs(mouseRay.pos, quat_lookat(vec3_zero, mouseRay.dir));
	}

	Registry<PulseReceiver> inRangeKeyReceivers;
	for(PulseReceiver *receiver : PulseReceiver::receivers.list()) {
		if(!receiver->field) continue;
		if(receiver->maskBinary != keyMask) continue;
		if(march(receiver->field.ptr()).distance > keySendDistance) continue;

		inRangeKeyReceivers.add(receiver);
	}

	sendKeyEvents(inRangeKeyReceivers, [&](flexbuffers::Builder &fbb){
		fbb.String("type", "keyboard");
		fbb.String("keymap", skKeymapString);
		fbb.Map("modifiers", [&](){
			uint32_t depressed = 0;
			uint32_t locked = 0;
			if(sk::input_key(key_shift) & button_state_active)
				depressed |= 1 << shiftIndex;
			if(sk::input_key(key_caps_lock) & button_state_active)
				locked |= 1 << capsIndex;
			if(sk::input_key(key_ctrl) & button_state_active)
				depressed |= 1 << ctrlIndex;
			if(sk::input_key(key_alt) & button_state_active)
				depressed |= 1 << altIndex;

			fbb.UInt("depressed", depressed);
			fbb.UInt("latched", 0);
			fbb.UInt("locked", locked);
			fbb.UInt("group", 0);
		});
	});

	for(int i=key_none; i<key_MAX; ++i) {
		button_state_ keyState = sk::input_key((key_) i);
		if(keyState & button_state_changed) {
			sendKeyEvents(inRangeKeyReceivers, [&](flexbuffers::Builder &fbb){
				fbb.String("type", "keyboard");
				fbb.UInt(keyState & button_state_just_active ? "pressed" : "released", i);
			});
		}
	}
}

void FlatscreenPointer::sendKeyEvents(Registry<PulseReceiver> &receivers, ArgsConstructor args) {
	std::vector<uint8_t> data = FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
		fbb.Map([&]() {
			args(fbb);
		});
	});

	for(PulseReceiver *receiver : receivers.list())
		receiver->sendData(&keySender, data);
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

	fbb.Float("select", left);
	fbb.Float("rotate", middle);
	fbb.Float("context", right);
}

} // namespace StardustXRServer
