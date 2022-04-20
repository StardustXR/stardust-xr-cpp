#include "xwaylandsurface.hpp"

extern "C" {
#include "xwaylandinclude.h"
}

XWaylandSurface::XWaylandSurface(Wayland *wayland, wlr_renderer *renderer, wlr_xwayland_surface *xwayland_surface, wlr_seat *seat) :
Surface(wayland, renderer, xwayland_surface->surface, seat) {
	this->xwayland_surface = xwayland_surface;

	if(wlr_surface_is_xwayland_surface(surface))
		printf("New XWayland surface created\n");

	wlr_xwayland_surface_activate(xwayland_surface, true);
	// wlr_xwayland_surface_set_fullscreen(xwayland_surface, true);
	wlr_xwayland_surface_set_maximized(xwayland_surface, true);
}

void XWaylandSurface::resize(uint32_t width, uint32_t height) const {
	wlr_xwayland_surface_configure(xwayland_surface, 0, 0, width, height);
}

void XWaylandSurface::close() const {
	wlr_xwayland_surface_close(xwayland_surface);
}

bool XWaylandSurface::isMapped() const {
	return xwayland_surface && xwayland_surface->mapped;
}
