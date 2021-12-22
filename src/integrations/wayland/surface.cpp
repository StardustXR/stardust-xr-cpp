#include <assert.h>

#include <stereokit.h>
#include "libraries/sk_gpu.h"
#include "asset_types/texture.h"

#include "surface.hpp"
#include "../../nodetypes/items/panel.hpp"
#include "../../nodetypes/items/itemui.hpp"
#include "../../globals.h"

extern "C" {
#include "render/egl.h"
#define static
#include "render/gles2.h"
#undef static
#include "wlr/types/wlr_surface.h"
#include "types/wlr_xdg_shell.h"
}

#include "shaders/sshader_unlit_gamma.hlsl.h"
#include "shaders/sshader_unlit_simula.hlsl.h"

using namespace sk;

Surface::Surface(wlr_renderer *renderer, wlr_surface *surface) {
	this->renderer = renderer;
	this->surface = surface;

	this->surfaceTex = tex_create(tex_type_image_nomips, tex_format_rgba32);

	this->surfaceTex->tex.type        = skg_tex_type_image;
	this->surfaceTex->tex.use         = skg_use_dynamic;
	this->surfaceTex->tex.mips        = skg_mip_none;
	this->surfaceTex->tex.format      = skg_tex_fmt_rgba32;
	this->surfaceTex->tex.array_count = 1;

//	this->surfaceShader = shader_create_mem((void *) sks_sshader_unlit_gamma_hlsl, 4032);
	this->surfaceShader = shader_create_mem((void *) sks_sshader_unlit_simula_hlsl, 6106);

	this->surfaceMatAlphaAdd   = material_create(this->surfaceShader);
	this->surfaceMatAlphaBlend = material_create(this->surfaceShader);
	this->surfaceMatAlphaClip  = material_create(this->surfaceShader);

	material_set_transparency(this->surfaceMatAlphaAdd,   transparency_add);
	material_set_transparency(this->surfaceMatAlphaBlend, transparency_blend);
	material_set_transparency(this->surfaceMatAlphaClip,  transparency_none);

	material_set_float(this->surfaceMatAlphaAdd,   "fcFactor", 0.5f);
	material_set_float(this->surfaceMatAlphaBlend, "fcFactor", 0.5f);
	material_set_float(this->surfaceMatAlphaClip,  "fcFactor", 0.5f);

	material_set_texture(this->surfaceMatAlphaAdd,   "diffuse", this->surfaceTex);
	material_set_texture(this->surfaceMatAlphaBlend, "diffuse", this->surfaceTex);
	material_set_texture(this->surfaceMatAlphaClip,  "diffuse", this->surfaceTex);

	surfaceCommitCallback.callback = std::bind(&Surface::onCommit, this);
	wl_signal_add(&surface->events.commit, &surfaceCommitCallback.listener);

	panel = new StardustXRServer::PanelItem(&serverInternalClient, this, sk::pose_identity);
	StardustXRServer::Node *internalPanelNode = serverInternalClient.scenegraph.findNode("/item/panel");
	if(internalPanelNode)
		internalPanelNode->addChild(std::to_string(panel->id), panel);
	if(StardustXRServer::PanelItem::itemTypeInfo.UI)
		StardustXRServer::PanelItem::itemTypeInfo.UI->handleItemCreate(panel);
}

Surface::~Surface() {
	tex_release(surfaceTex);
	shader_release(surfaceShader);
	material_release(surfaceMatAlphaAdd);
	material_release(surfaceMatAlphaBlend);
	material_release(surfaceMatAlphaClip);

	panel->queueDestroy(true);
}

void Surface::onCommit() {
	wlr_texture *surfaceTexture = wlr_surface_get_texture(surface);
	if(!surfaceTexture || !wlr_texture_is_gles2(surfaceTexture))
		return;
	wlr_gles2_texture *eglTexture = (wlr_gles2_texture *) surfaceTexture;

	this->width  = surface->current.width;
	this->height = surface->current.height;

	this->surfaceTex->tex.width       = surfaceTexture->width;
	this->surfaceTex->tex.height      = surfaceTexture->height;
	this->surfaceTex->tex._texture    = eglTexture->tex;
	this->surfaceTex->tex._target     = eglTexture->target;

	tex_set_options(surfaceTex, sk::tex_sample_point, tex_address_clamp, 1);

	int pixels[2] = {
		(int) surfaceTexture->width,
		(int) surfaceTexture->height
	};
	material_set_param(this->surfaceMatAlphaAdd,   "size", material_param_float, (void *) pixels);
	material_set_param(this->surfaceMatAlphaBlend, "size", material_param_float, (void *) pixels);
	material_set_param(this->surfaceMatAlphaClip,  "size", material_param_float, (void *) pixels);

//	tex_set_surface(
//		this->surfaceTex,
//		(void *)(uintptr_t) eglTexture->tex,
//		tex_type_image_nomips,
//		eglTexture->drm_format,
//		surfaceTexture->width,
//		surfaceTexture->height,
//		1
//	);

	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	wlr_surface_send_frame_done(surface, &now);
}
