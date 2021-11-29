#pragma once

#include "surface.hpp"

class XWaylandSurface : public Surface {
public:
    explicit XWaylandSurface(struct wlr_renderer *renderer, struct wlr_xwayland_surface *surface);
	~XWaylandSurface();

	struct wlr_xwayland_surface *xwayland_surface;
	bool isMapped() const override;
};
