#include "xdgsurface.hpp"

extern "C" {
#include "types/wlr_xdg_shell.h"
#include "wlr/util/edges.h"
}

XDGSurface::XDGSurface(wl_display *display, wlr_renderer *renderer, wlr_xdg_surface *xdg_surface) : Surface(display, renderer, xdg_surface->surface) {
	this->xdg_surface = xdg_surface;

	if(wlr_surface_is_xdg_surface(surface))
		printf("New XDG surface created\n");

	wlr_xdg_toplevel_set_activated(xdg_surface, true);
	// wlr_xdg_toplevel_set_fullscreen(xdg_surface, true);
	wlr_xdg_toplevel_set_maximized(xdg_surface, true);
	wlr_xdg_toplevel_set_tiled(xdg_surface, WLR_EDGE_TOP | WLR_EDGE_BOTTOM | WLR_EDGE_RIGHT | WLR_EDGE_LEFT);
}

void XDGSurface::resize(uint32_t width, uint32_t height) const {
	wlr_xdg_toplevel_set_size(xdg_surface, width, height);
}

void XDGSurface::close() const {
	wlr_xdg_toplevel_send_close(xdg_surface);
}

bool XDGSurface::isMapped() const {
	return xdg_surface && xdg_surface->mapped;
}
