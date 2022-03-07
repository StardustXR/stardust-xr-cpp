#pragma once

#include <memory>
#include <vector>

#include <EGL/egl.h>
#include <wayland-server.h>

#include "callbacks.h"
#include "xdgsurface.hpp"
#include "xwaylandsurface.hpp"

struct wlr_egl;
struct wlr_compositor;
struct wlr_data_device_manager;
struct wlr_backend;
struct wlr_output;
struct wlr_output_layout;
struct wlr_xdg_decoration_manager_v1;
struct wlr_xdg_shell;
struct wlr_xwayland;

class Wayland {
public:
	explicit Wayland(EGLDisplay display, EGLContext context, EGLenum platform);
	~Wayland();

	void update();

	wlr_seat *createSeat();

	void onNewSurface(Surface *surface);

	void onNewXDGSurface(void *data);
	void onNewToplevelDecorationXDG(void *data);

	void onNewXWaylandSurface(void *data);
	void onMapXWaylandSurface(void *data);

protected:
	wl_display *wayland_display;
	wl_event_loop *event_loop;

	wlr_egl *egl;
	wlr_renderer *renderer;
	wlr_compositor *compositor;
	wlr_backend *backend;
	wlr_output *output;

	wlr_data_device_manager *data_device;
	xkb_keymap *keymap;
	wlr_seat *queueSeat;
	uint32_t seatID = 0;

	wlr_xdg_shell *xdg_shell;
	WaylandCallback newSurfaceCallbackXDG;

	wlr_xdg_decoration_manager_v1 *xdg_decoration_manager;
	WaylandCallback newToplevelDecorationXDG;

	wlr_xwayland *xwayland;
	WaylandCallback newSurfaceCallbackXWayland;
	WaylandCallback mapSurfaceCallbackXWayland;
};
