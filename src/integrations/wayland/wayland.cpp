#include "assert.h"
#include <algorithm>
#include <EGL/egl.h>
#include <string>
#include <stereokit.h>
#include <wayland-client-core.h>

#include "callbacks.h"
#include "wayland.hpp"

extern "C" {

#define static
#include "wlr/types/wlr_compositor.h"
#include "wlr/render/gles2.h"
#include "wlr/render/egl.h"
#include "wlr/backend/headless.h"
#undef static

#include "wlr/types/wlr_xdg_decoration_v1.h"
#include "wlr/types/wlr_xdg_decoration_v1.h"
#include "wlr/types/wlr_data_device.h"
#include "wlr/types/wlr_output.h"
#include "wlr/types/wlr_output_layout.h"
#include "wlr/types/wlr_xdg_shell.h"
#include "xwaylandinclude.h"

#include "wlr/util/log.h"

}

static void wlr_log_handler(wlr_log_importance level, const char *fmt, va_list args) {
	std::string format = "[WLR]";

	switch (level) {
		case WLR_ERROR:
			format += "[ERROR]";
		break;

//		case WLR_INFO:
//			format += "[INFO]";
//		break;

//		case WLR_DEBUG:
//			format += "[DEBUG]";
//		break;

		default:
	return;
	}

	format += fmt;
	format += "\n";

	vprintf(format.c_str(), args);
}

Wayland::Wayland(EGLDisplay display, EGLContext context) {
	wlr_log_init(WLR_DEBUG, wlr_log_handler);

	wayland_display = wl_display_create();
	assert(wayland_display);

	event_loop = wl_display_get_event_loop(wayland_display);
	assert(event_loop);

	const char *socket = wl_display_add_socket_auto(wayland_display);
	assert(socket);

	printf("Running compositor on wayland display '%s'\n", socket);
	setenv("WAYLAND_DISPLAY", socket, true);
	
	// wl_display_init_shm(display);
	egl = wlr_egl_create_with_context(display, context);
	assert(egl);

	renderer = wlr_gles2_renderer_create(egl);
	assert(renderer);
	wlr_egl_make_current(egl);
	wlr_renderer_init_wl_display(renderer, wayland_display);

	compositor = wlr_compositor_create(wayland_display, renderer);
	assert(compositor);

	backend = wlr_headless_backend_create(wayland_display);
	assert(backend);
	if (!wlr_backend_start(backend)) {
		fprintf(stderr, "Failed to start backend\n");
		wl_display_destroy(wayland_display);
		exit(1);
	}

	output = wlr_headless_add_output(backend, 0, 0);
	assert(output);
	wlr_output_set_scale(output, 2.0f);
	wlr_output_set_subpixel(output, WL_OUTPUT_SUBPIXEL_NONE);
	wlr_output_commit(output);
	wlr_output_create_global(output);

	data_device = wlr_data_device_manager_create(wayland_display);
	{
		xkb_context *kb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
		xkb_rule_names rule_names = {0};
		keymap = xkb_keymap_new_from_names(kb_ctx, &rule_names, XKB_KEYMAP_COMPILE_NO_FLAGS);
		xkb_context_unref(kb_ctx);
	}
	queueSeat = createSeat();

	xdg_shell = wlr_xdg_shell_create(wayland_display);
	assert(xdg_shell);
	newSurfaceCallbackXDG.callback = std::bind(&Wayland::onNewXDGSurface, this, std::placeholders::_1);
	wl_signal_add(&xdg_shell->events.new_surface, &newSurfaceCallbackXDG.listener);

	xdg_decoration_manager = wlr_xdg_decoration_manager_v1_create(wayland_display);
	assert(xdg_decoration_manager);
	wl_signal_add(&xdg_decoration_manager->events.new_toplevel_decoration, &newToplevelDecorationXDG.listener);
	newToplevelDecorationXDG.callback = std::bind(&Wayland::onNewToplevelDecorationXDG, this, std::placeholders::_1);

	xwayland = wlr_xwayland_create(wayland_display, compositor, false);
	assert(xwayland);
	newSurfaceCallbackXWayland.callback = std::bind(&Wayland::onNewXWaylandSurface, this, std::placeholders::_1);
	wl_signal_add(&xwayland->events.new_surface, &newSurfaceCallbackXWayland.listener);
	mapSurfaceCallbackXWayland.callback = std::bind(&Wayland::onMapXWaylandSurface, this, std::placeholders::_1);
}

Wayland::~Wayland() {
    wlr_seat_destroy(queueSeat);
	wlr_xwayland_destroy(xwayland);
	wl_display_destroy_clients(wayland_display);
	wl_display_destroy(wayland_display);
    wlr_renderer_destroy(renderer);

}

bool Wayland::dispatch() {
	std::lock_guard<std::mutex> lock(mutex);

	int dispatch_status = wl_event_loop_dispatch(event_loop, 1);
	if(dispatch_status == -1) return false;

	for(Surface *surface : surfaces)
		surface->frameEnd();

	wl_display_flush_clients(wayland_display);
	return true;
}

wlr_seat *Wayland::createSeat() {
	wlr_seat *seat = wlr_seat_create(wayland_display, std::to_string(seatID).c_str());
	wlr_input_device *keyboard_device = (wlr_input_device *)calloc (1, sizeof (wlr_input_device));
	wlr_input_device_init(keyboard_device, WLR_INPUT_DEVICE_KEYBOARD, nullptr, seat->name, 0, 0);

	wlr_keyboard *keyboard = (wlr_keyboard *)calloc (1, sizeof (wlr_keyboard));
	wlr_keyboard_init(keyboard, nullptr);
	keyboard_device->keyboard = keyboard;
	wlr_keyboard_set_keymap(keyboard, keymap);
	wlr_keyboard_set_repeat_info(keyboard, 25, 600);
	wlr_seat_set_keyboard(seat, keyboard_device);
	seatID++;
	return seat;
}

void Wayland::onNewSurface(Surface *surface) {
	queueSeat = createSeat();
	seatID++;
}

void Wayland::onNewXDGSurface(void *data) {
	wlr_xdg_surface *surface = (wlr_xdg_surface *) data;
	if(surface->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL)
		return;

	XDGSurface *newSurface = new XDGSurface(this, renderer, surface, queueSeat);
	surfaces.push_back(newSurface);
	newSurface->destroyCallback.callback = [this, newSurface](void *data) {
		surfaces.erase(std::remove(surfaces.begin(), surfaces.end(), newSurface));
		delete newSurface;
	};
}

void Wayland::onNewToplevelDecorationXDG(void *data) {
	wlr_xdg_toplevel_decoration_v1 *decoration = (wlr_xdg_toplevel_decoration_v1 *) data;
	wlr_xdg_toplevel_decoration_v1_set_mode(decoration, WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
}

void Wayland::onNewXWaylandSurface(void *data) {
	wlr_xwayland_surface *surface = (wlr_xwayland_surface *) data;
	wl_signal_add(&surface->events.map, &mapSurfaceCallbackXWayland.listener);
}
void Wayland::onMapXWaylandSurface(void *data) {
	wlr_xwayland_surface *surface = (wlr_xwayland_surface *) data;

	XWaylandSurface *newSurface = new XWaylandSurface(this, renderer, surface, queueSeat);
	surfaces.push_back(newSurface);
	newSurface->destroyCallback.callback = [this, newSurface](void *data) {
		surfaces.erase(std::remove(surfaces.begin(), surfaces.end(), newSurface));
		delete newSurface;
	};

	onNewSurface(newSurface);
}
