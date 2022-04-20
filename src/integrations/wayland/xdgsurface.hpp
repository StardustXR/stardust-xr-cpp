#pragma once

#include "surface.hpp"

class XDGSurface : public Surface {
public:
	XDGSurface(Wayland *wayland, wlr_renderer *renderer, struct wlr_xdg_surface *surface, wlr_seat *seat);

	wlr_xdg_surface *xdg_surface;
	void resize(uint32_t width, uint32_t height) const override;
	void close() const override;
	bool isMapped() const override;
    ~XDGSurface();
};
