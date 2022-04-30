#include <assert.h>

#include <stereokit.h>
#include "libraries/sk_gpu.h"
#include "asset_types/texture.h"

#include "surface.hpp"
#include "wayland.hpp"
#include "../../nodetypes/items/types/panel.hpp"
#include "../../nodetypes/items/itemui.hpp"
#include "../../globals.h"
#include "../../util/time.hpp"

extern "C" {
#include "render/egl.h"
#define static
#include "render/gles2.h"
#undef static
#include "wlr/types/wlr_surface.h"
#include "wlr/types/wlr_seat.h"
#include "wlr/interfaces/wlr_keyboard.h"
#include "wlr/types/wlr_xdg_shell.h"

#include <xkbcommon/xkbcommon.h>
}

#include "shaders/sshader_unlit_gamma.hlsl.h"
#include "shaders/sshader_unlit_simula.hlsl.h"

using namespace sk;

Surface::Surface(Wayland *wayland, wlr_renderer *renderer, wlr_surface *surface, wlr_seat *seat) :
surface(surface),
wayland(wayland),
renderer(renderer),
seat(seat) {
	this->surfaceTex = tex_create(tex_type_image_nomips, tex_format_rgba32);

	this->surfaceTex->tex.type        = skg_tex_type_image;
	this->surfaceTex->tex.use         = skg_use_dynamic;
	this->surfaceTex->tex.mips        = skg_mip_none;
	this->surfaceTex->tex.format      = skg_tex_fmt_rgba32;
	this->surfaceTex->tex.array_count = 1;
	this->surfaceTex->header.state    = asset_state_loading;

//	this->surfaceShader = shader_create_mem((void *) sks_sshader_unlit_gamma_hlsl, sizeof(sks_sshader_unlit_gamma_hlsl)/sizeof(sks_sshader_unlit_gamma_hlsl[0]));
	this->surfaceShader = shader_create_mem((void *) sks_sshader_unlit_simula_hlsl, sizeof(sks_sshader_unlit_simula_hlsl)/sizeof(sks_sshader_unlit_simula_hlsl[0]));

	material_t surfaceMatBase = material_create(this->surfaceShader);
	material_set_float(surfaceMatBase,  "fcFactor", 0.5f);
	material_set_texture(surfaceMatBase,  "diffuse", this->surfaceTex);

	this->surfaceMatAlphaAdd   = material_copy(surfaceMatBase);
	material_set_transparency(this->surfaceMatAlphaAdd,   transparency_add);
	this->surfaceMatAlphaBlend = material_copy(surfaceMatBase);
	material_set_transparency(this->surfaceMatAlphaBlend, transparency_blend);
	this->surfaceMatAlphaClip  = material_copy(surfaceMatBase);
	material_set_transparency(this->surfaceMatAlphaClip,  transparency_none);

	material_release(surfaceMatBase);


	surfaceCommitCallback.callback = std::bind(&Surface::onCommit, this);
	wl_signal_add(&surface->events.commit, &surfaceCommitCallback.listener);

	panel = new StardustXRServer::PanelItem(nullptr, this, sk::pose_identity);
	serverInternalNode->addChild("panel"+std::to_string(panel->id), panel);
	std::string panelName = std::to_string(panel->id);

	wlr_seat_set_capabilities(seat, WL_SEAT_CAPABILITY_POINTER | WL_SEAT_CAPABILITY_KEYBOARD | WL_SEAT_CAPABILITY_TOUCH);

	kb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	keyboard = wlr_seat_get_keyboard(seat);

	wl_signal_add(&surface->events.destroy, &destroyCallback.listener);

	if(StardustXRServer::PanelItem::itemTypeInfo.UI)
		StardustXRServer::PanelItem::itemTypeInfo.UI->handleItemCreate(panel);
}

Surface::~Surface() {
	panel->queueDestroy(true);

//	wlr_seat_destroy(seat);

	tex_release(surfaceTex);
	shader_release(surfaceShader);
	material_release(surfaceMatAlphaAdd);
	material_release(surfaceMatAlphaBlend);
	material_release(surfaceMatAlphaClip);
}

void Surface::frameEnd() {
	wlr_seat_pointer_send_frame(seat);
	wlr_seat_touch_send_frame(seat);
}

void Surface::onCommit() {
	wlr_texture *surfaceTexture = wlr_surface_get_texture(surface);
	if(!surfaceTexture || !wlr_texture_is_gles2(surfaceTexture))
		return;
	wlr_gles2_texture *eglTexture = (wlr_gles2_texture *) surfaceTexture;

	this->width  = surface->current.width;
	this->height = surface->current.height;

	this->surfaceTex->tex.width       = surfaceTexture->width;
	this->surfaceTex->tex.height      = surfaceTexture->height;
	this->surfaceTex->tex._texture    = eglTexture->tex;
	this->surfaceTex->tex._target     = eglTexture->target;
	this->surfaceTex->header.state    = asset_state_loaded_meta;
	this->surfaceTex->fallback        = nullptr;

	tex_set_options(surfaceTex, sk::tex_sample_point, tex_address_clamp, 1);

	int pixels[2] = {
		(int) surfaceTexture->width,
		(int) surfaceTexture->height
	};
	material_set_param(this->surfaceMatAlphaAdd,   "size", material_param_float, (void *) pixels);
	material_set_param(this->surfaceMatAlphaBlend, "size", material_param_float, (void *) pixels);
	material_set_param(this->surfaceMatAlphaClip,  "size", material_param_float, (void *) pixels);

//	tex_set_surface(
//		this->surfaceTex,
//		(void *)(uintptr_t) eglTexture->tex,
//		tex_type_image_nomips,
//		eglTexture->drm_format,
//		surfaceTexture->width,
//		surfaceTexture->height,
//		1
//	);

	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	wlr_surface_send_frame_done(surface, &now);
}

void Surface::setPointerActive(bool active) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	if(active && isMapped())
		wlr_seat_pointer_enter(seat, surface, seat->pointer_state.sx, seat->pointer_state.sy);
	else
		wlr_seat_pointer_clear_focus(seat);
}
void Surface::setPointerPosition(double x, double y) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	wlr_seat_pointer_send_motion(seat, StardustXRServer::Time::timestampMS(), x, y);
}
void Surface::setPointerButtonPressed(uint32_t button, bool pressed) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	wlr_seat_pointer_send_button(seat, StardustXRServer::Time::timestampMS(), button, pressed ? WLR_BUTTON_PRESSED : WLR_BUTTON_RELEASED);
}
void Surface::scrollPointerAxis(uint32_t source, double x, double y, int32_t dx, int32_t dy) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	if(x != 0 || dx != 0)
		wlr_seat_pointer_send_axis(seat, StardustXRServer::Time::timestampMS(), WLR_AXIS_ORIENTATION_HORIZONTAL,  x,  dx, (wlr_axis_source) source);
	if(y != 0 || dy != 0)
		wlr_seat_pointer_send_axis(seat, StardustXRServer::Time::timestampMS(), WLR_AXIS_ORIENTATION_VERTICAL  , -y, -dy, (wlr_axis_source) source);
}

void Surface::touchDown(uint32_t id, double x, double y) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	wlr_seat_touch_send_down(seat, surface, StardustXRServer::Time::timestampMS(), id, x, y);
}
void Surface::touchMove(uint32_t id, double x, double y) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	wlr_seat_touch_send_motion(seat, StardustXRServer::Time::timestampMS(), id, x, y);
}
void Surface::touchUp(uint32_t id) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	wlr_seat_touch_send_up(seat, StardustXRServer::Time::timestampMS(), id);
}

void Surface::setKeyboardActive(bool active) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	if(active) {
		wlr_seat_keyboard_enter(seat, surface, keyboard->keycodes, keyboard->num_keycodes, &keyboard->modifiers);
	} else {
		wlr_seat_keyboard_clear_focus(seat);
	}
}
void Surface::setKeymap(std::string keymapString) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	xkb_keymap *keymap = xkb_keymap_new_from_string(kb_context, keymapString.c_str(), XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	wlr_keyboard_set_keymap(keyboard, keymap);
	xkb_keymap_unref(keymap);
}
void Surface::setKeyState(uint32_t key, uint32_t state) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	wlr_seat_keyboard_send_key(seat, StardustXRServer::Time::timestampMS(), key-8, state);
}
void Surface::setKeyModStates(uint32_t depressed, uint32_t latched, uint32_t locked, uint32_t group) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	struct wlr_keyboard_modifiers mods{depressed, latched, locked, group};
	wlr_seat_keyboard_send_modifiers(seat, &mods);
}
void Surface::setKeyRepeat(int32_t rate, int32_t delay) {
	std::lock_guard<std::mutex> lock(wayland->mutex);
	wlr_keyboard_set_repeat_info(keyboard, rate, delay);
}
