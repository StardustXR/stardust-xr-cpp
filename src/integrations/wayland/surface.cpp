#include <assert.h>

#include <stereokit.h>
#include "../sk_internal_defs.hpp"

#include "surface.hpp"

extern "C" {
#include "render/egl.h"
#define static
#include "render/gles2.h"
#undef static
#include "wlr/types/wlr_surface.h"
#include "types/wlr_xdg_shell.h"
}

#include "shaders/shader_unlit_gamma.sks.h"
#include "shaders/shader_unlit_simula.sks.h"

using namespace sk;

Surface::Surface(wlr_renderer *renderer, wlr_surface *surface) {
	this->renderer = renderer;
	this->surface = surface;

	this->surfaceTex = tex_create();
	this->surfaceTex->type            = tex_type_image_nomips;
	this->surfaceTex->format          = tex_format_rgba32;

	this->surfaceTex->tex.type        = skg_tex_type_image;
	this->surfaceTex->tex.use         = skg_use_dynamic;
	this->surfaceTex->tex.mips        = skg_mip_none;
	this->surfaceTex->tex.format      = skg_tex_fmt_rgba32;
	this->surfaceTex->tex.array_count = 1;

	// this->surfaceShader = shader_create_mem((void *) shader_unlit_gamma_sks, shader_unlit_gamma_sks_size);
	this->surfaceShader = shader_create_mem((void *) shader_unlit_simula_sks, shader_unlit_simula_sks_size);

	this->surfaceMat = material_create(this->surfaceShader);
	material_set_transparency(this->surfaceMat, transparency_blend);
	material_set_texture(this->surfaceMat, "diffuse", this->surfaceTex);

	surfaceCommitCallback.callback = std::bind(&Surface::onCommit, this);
	wl_signal_add(&surface->events.commit, &surfaceCommitCallback.listener);
}

Surface::~Surface() {
	tex_release(surfaceTex);
	shader_release(surfaceShader);
	material_release(surfaceMat);
}

void Surface::onCommit() {
	wlr_texture *surfaceTexture = wlr_surface_get_texture(surface);
	if(!surfaceTexture || !wlr_texture_is_gles2(surfaceTexture))
		return;
	wlr_gles2_texture *eglTexture = (wlr_gles2_texture *) surfaceTexture;

	this->width                       = surfaceTexture->width;
	this->height                      = surfaceTexture->height;
	this->density                     = surface->current.scale;

	this->surfaceTex->tex.width       = surfaceTexture->width;
	this->surfaceTex->tex.height      = surfaceTexture->height;
	this->surfaceTex->tex._texture    = eglTexture->tex;
	this->surfaceTex->tex._target     = eglTexture->target;
	tex_set_options(this->surfaceTex, sk::tex_sample_point, tex_address_clamp, 1);

	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	wlr_surface_send_frame_done(surface, &now);
}
