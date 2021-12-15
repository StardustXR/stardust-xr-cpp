#pragma once

#include "surface.hpp"

class XDGSurface : public Surface {
public:
    explicit XDGSurface(struct wlr_renderer *renderer, struct wlr_xdg_surface *surface);

	struct wlr_xdg_surface *xdg_surface;
	void close() const override;
	bool isMapped() const override;
};
