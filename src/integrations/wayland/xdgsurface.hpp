#pragma once

#include "surface.hpp"

class XDGSurface : public Surface {
public:
    explicit XDGSurface(struct wlr_renderer *renderer, struct wlr_xdg_surface *surface);
	~XDGSurface();

	struct wlr_xdg_surface *xdg_surface;
	bool isMapped() const override;
};
