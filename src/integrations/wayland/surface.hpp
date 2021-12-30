#pragma once
#include "callbacks.h"
#include <string>
#include <stereokit.h>

struct wlr_renderer;
struct wlr_surface;
struct wlr_seat;
struct wlr_keyboard;

namespace StardustXRServer {
class PanelItem;
}

class Wayland;

class Surface {
public:
	explicit Surface(wl_display *display, wlr_renderer *renderer, wlr_surface *surface, wlr_seat *seat);
	virtual ~Surface();

	void frameEnd();
	void onCommit();
	virtual void resize(uint32_t width, uint32_t height) const = 0;
	virtual void close() const = 0;

	void setPointerActive(bool active);
	void setPointerPosition(double x, double y);
	void setPointerButtonPressed(uint32_t button, bool pressed);
	void scrollPointerAxis(uint32_t source, double x, double y, int32_t dx, int32_t dy);

	void touchDown(uint32_t id, double x, double y);
	void touchMove(uint32_t id, double x, double y);
	void touchUp(uint32_t id);

	void setKeyboardActive(bool active);
	void setKeyState(uint32_t key, uint32_t state);

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

	bool pointerActive = false;

    wlr_renderer *renderer;
	wlr_seat *seat;
	wlr_keyboard *keyboard;
	std::unordered_map<uint32_t, bool> buttonStates;

	bool updateSurface();
	sk::shader_t surfaceShader;

	StardustXRServer::PanelItem *panel;
};
