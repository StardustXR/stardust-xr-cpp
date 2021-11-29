#include "xwaylandsurface.hpp"

extern "C" {
#include "xwaylandinclude.h"
}

XWaylandSurface::XWaylandSurface(wlr_renderer *renderer, wlr_xwayland_surface *xwayland_surface) : Surface(renderer, xwayland_surface->surface) {
	this->xwayland_surface = xwayland_surface;

	if(wlr_surface_is_xwayland_surface(surface))
		printf("New XWayland surface created\n");

	wlr_xwayland_surface_activate(xwayland_surface, true);
	// wlr_xwayland_surface_set_fullscreen(xwayland_surface, true);
	wlr_xwayland_surface_set_maximized(xwayland_surface, true);
}

XWaylandSurface::~XWaylandSurface() {}

bool XWaylandSurface::isMapped() const {
	return xwayland_surface && xwayland_surface->mapped;
}