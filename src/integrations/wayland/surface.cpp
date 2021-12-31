#include <assert.h>

#include <stereokit.h>
#include "libraries/sk_gpu.h"
#include "asset_types/texture.h"

#include "surface.hpp"
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
#include "types/wlr_xdg_shell.h"

#include <xkbcommon/xkbcommon.h>
}

#include "shaders/sshader_unlit_gamma.hlsl.h"
#include "shaders/sshader_unlit_simula.hlsl.h"

using namespace sk;

Surface::Surface(wl_display *display, wlr_renderer *renderer, wlr_surface *surface, wlr_seat *seat) {
	this->renderer = renderer;
	this->surface = surface;

	this->surfaceTex = tex_create(tex_type_image_nomips, tex_format_rgba32);

	this->surfaceTex->tex.type        = skg_tex_type_image;
	this->surfaceTex->tex.use         = skg_use_dynamic;
	this->surfaceTex->tex.mips        = skg_mip_none;
	this->surfaceTex->tex.format      = skg_tex_fmt_rgba32;
	this->surfaceTex->tex.array_count = 1;

//	this->surfaceShader = shader_create_mem((void *) sks_sshader_unlit_gamma_hlsl, 4032);
	this->surfaceShader = shader_create_mem((void *) sks_sshader_unlit_simula_hlsl, 6106);

	this->surfaceMatAlphaAdd   = material_create(this->surfaceShader);
	this->surfaceMatAlphaBlend = material_create(this->surfaceShader);
	this->surfaceMatAlphaClip  = material_create(this->surfaceShader);

	material_set_transparency(this->surfaceMatAlphaAdd,   transparency_add);
	material_set_transparency(this->surfaceMatAlphaBlend, transparency_blend);
	material_set_transparency(this->surfaceMatAlphaClip,  transparency_none);

	material_set_float(this->surfaceMatAlphaAdd,   "fcFactor", 0.5f);
	material_set_float(this->surfaceMatAlphaBlend, "fcFactor", 0.5f);
	material_set_float(this->surfaceMatAlphaClip,  "fcFactor", 0.5f);

	material_set_texture(this->surfaceMatAlphaAdd,   "diffuse", this->surfaceTex);
	material_set_texture(this->surfaceMatAlphaBlend, "diffuse", this->surfaceTex);
	material_set_texture(this->surfaceMatAlphaClip,  "diffuse", this->surfaceTex);

	surfaceCommitCallback.callback = std::bind(&Surface::onCommit, this);
	wl_signal_add(&surface->events.commit, &surfaceCommitCallback.listener);

	panel = new StardustXRServer::PanelItem(&serverInternalClient, this, sk::pose_identity);
	StardustXRServer::Node *internalPanelNode = serverInternalClient.scenegraph.findNode("/item/panel");
	std::string panelName = std::to_string(panel->id);

	this->seat = seat;
	wlr_seat_set_capabilities(seat, WL_SEAT_CAPABILITY_POINTER | WL_SEAT_CAPABILITY_TOUCH);
//	wlr_seat_set_capabilities(seat, WL_SEAT_CAPABILITY_POINTER | WL_SEAT_CAPABILITY_KEYBOARD | WL_SEAT_CAPABILITY_TOUCH);

	if(internalPanelNode)
		internalPanelNode->addChild(panelName, panel);
	if(StardustXRServer::PanelItem::itemTypeInfo.UI)
		StardustXRServer::PanelItem::itemTypeInfo.UI->handleItemCreate(panel);
}

Surface::~Surface() {
	wlr_seat_destroy(seat);

	tex_release(surfaceTex);
	shader_release(surfaceShader);
	material_release(surfaceMatAlphaAdd);
	material_release(surfaceMatAlphaBlend);
	material_release(surfaceMatAlphaClip);

	panel->queueDestroy(true);
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
	if(active)
		wlr_seat_pointer_enter(seat, surface, seat->pointer_state.sx, seat->pointer_state.sy);
	else
		wlr_seat_pointer_clear_focus(seat);
}
void Surface::setPointerPosition(double x, double y) {
	wlr_seat_pointer_send_motion(seat, StardustXRServer::Time::timestampMS(), x, y);
}
void Surface::setPointerButtonPressed(uint32_t button, bool pressed) {
	if(buttonStates.find(button) != buttonStates.end() && buttonStates[button] == pressed)
		return;
	wlr_seat_pointer_notify_button(seat, StardustXRServer::Time::timestampMS(), button, pressed ? WLR_BUTTON_PRESSED : WLR_BUTTON_RELEASED);
	buttonStates[button] = pressed;
}
void Surface::scrollPointerAxis(uint32_t source, double x, double y, int32_t dx, int32_t dy) {
	if(x != 0 || dx != 0)
		wlr_seat_pointer_send_axis(seat, StardustXRServer::Time::timestampMS(), WLR_AXIS_ORIENTATION_HORIZONTAL,  x,  dx, (wlr_axis_source) source);
	if(y != 0 || dy != 0)
		wlr_seat_pointer_send_axis(seat, StardustXRServer::Time::timestampMS(), WLR_AXIS_ORIENTATION_VERTICAL  , -y, -dy, (wlr_axis_source) source);
}

void Surface::touchDown(uint32_t id, double x, double y) {
	wlr_seat_touch_notify_down(seat, surface, StardustXRServer::Time::timestampMS(), id, x, y);
}
void Surface::touchMove(uint32_t id, double x, double y) {
	wlr_seat_touch_notify_motion(seat, StardustXRServer::Time::timestampMS(), id, x, y);
}
void Surface::touchUp(uint32_t id) {
	wlr_seat_touch_notify_up(seat, StardustXRServer::Time::timestampMS(), id);
}

void Surface::setKeyboardActive(bool active) {
	if(active) {
		wlr_keyboard *keyboard = wlr_seat_get_keyboard(seat);
		wlr_seat_keyboard_enter(seat, surface, keyboard->keycodes, keyboard->num_keycodes, &keyboard->modifiers);
	} else {
		wlr_seat_keyboard_clear_focus(seat);
	}
}
