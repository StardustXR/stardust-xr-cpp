#pragma once
#include "callbacks.h"
#include <stereokit.h>

struct wlr_renderer;
struct wlr_surface;

namespace StardustXRServer {
class PanelItem;
}

class Wayland;

class Surface {
public:
	explicit Surface(wlr_renderer *renderer, wlr_surface *surface);
	virtual ~Surface();

	void onCommit();
	virtual void resize(uint32_t width, uint32_t height) const = 0;
	virtual void close() const = 0;

	virtual bool isMapped() const = 0;
	sk::tex_t surfaceTex;
	sk::material_t surfaceMatAlphaClip;
	sk::material_t surfaceMatAlphaAdd;
	sk::material_t surfaceMatAlphaBlend;
	uint width;
	uint height;
	wlr_surface *surface;

protected:
	WaylandCallback surfaceCommitCallback;

    wlr_renderer *renderer;

	bool updateSurface();
	sk::shader_t surfaceShader;

	StardustXRServer::PanelItem *panel;
};
