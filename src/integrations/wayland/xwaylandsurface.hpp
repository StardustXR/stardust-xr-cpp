#pragma once

#include "surface.hpp"

class XWaylandSurface : public Surface {
public:
	XWaylandSurface(Wayland *wayland, wlr_renderer *renderer, struct wlr_xwayland_surface *surface, wlr_seat *seat);

	struct wlr_xwayland_surface *xwayland_surface;
	void resize(uint32_t width, uint32_t height) const override;
	void close() const override;
	bool isMapped() const override;
};
