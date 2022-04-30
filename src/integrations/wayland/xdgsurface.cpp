#include "xdgsurface.hpp"

extern "C" {
#include "types/wlr_xdg_shell.h"
#include "wlr/util/edges.h"
}

XDGSurface::XDGSurface(Wayland *wayland, wlr_renderer *renderer, wlr_xdg_surface *xdg_surface, wlr_seat *seat) :
Surface(wayland, renderer, xdg_surface->surface, seat),
xdg_surface(xdg_surface) {
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

XDGSurface::~XDGSurface() {
    //if(xdg_surface != NULL)
    //wlr_xdg_popup_destroy(xdg_surface);
}

bool XDGSurface::isMapped() const {
	return xdg_surface && xdg_surface->mapped;
}
