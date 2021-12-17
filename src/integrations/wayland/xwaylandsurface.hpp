#pragma once

#include "surface.hpp"

class XWaylandSurface : public Surface {
public:
	explicit XWaylandSurface(struct wlr_renderer *renderer, struct wlr_xwayland_surface *surface);

	struct wlr_xwayland_surface *xwayland_surface;
	void resize(uint32_t width, uint32_t height) const override;
	void close() const override;
	bool isMapped() const override;
};
