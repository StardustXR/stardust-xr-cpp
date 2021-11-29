#pragma once

#include "stereokit.h"

namespace sk {

	typedef struct skg_platform_data_t {
		void *_egl_display;
		void *_egl_config;
		void *_egl_context;
	} skg_platform_data_t;

	enum asset_type_ {
		asset_type_mesh = 0,
		asset_type_texture,
		asset_type_shader,
		asset_type_material,
		asset_type_model,
		asset_type_font,
		asset_type_sprite,
		asset_type_sound,
	};
	struct asset_header_t {
		asset_type_ type;
		uint64_t    id;
		int32_t     refs;
		uint64_t    index;
	#ifdef _DEBUG
		char       *id_text;
	#endif
	};


	typedef enum skg_use_ {
		skg_use_static,
		skg_use_dynamic,
	} skg_use_;
	typedef enum skg_tex_type_ {
		skg_tex_type_image,
		skg_tex_type_cubemap,
		skg_tex_type_rendertarget,
		skg_tex_type_depth,
	} skg_tex_type_;
	typedef enum skg_tex_fmt_ {
		skg_tex_fmt_none = 0,
		skg_tex_fmt_rgba32,
		skg_tex_fmt_rgba32_linear,
		skg_tex_fmt_bgra32,
		skg_tex_fmt_bgra32_linear,
		skg_tex_fmt_rgba64,
		skg_tex_fmt_rgba128,
		skg_tex_fmt_r8,
		skg_tex_fmt_r16,
		skg_tex_fmt_r32,
		skg_tex_fmt_depthstencil,
		skg_tex_fmt_depth32,
		skg_tex_fmt_depth16,
	} skg_tex_fmt_;
	typedef enum skg_mip_ {
		skg_mip_generate,
		skg_mip_none,
	} skg_mip_;
	typedef struct skg_tex_t {
		int32_t       width;
		int32_t       height;
		int32_t       array_count;
		int32_t       array_start;
		skg_use_      use;
		skg_tex_type_ type;
		skg_tex_fmt_  format;
		skg_mip_      mips;
		uint32_t      _texture;
		uint32_t      _framebuffer;
		uint32_t      _target;
	} skg_tex_t;
	
	struct _tex_t {
		asset_header_t header;
		tex_format_    format;
		tex_type_      type;
		tex_sample_    sample_mode;
		tex_address_   address_mode;
		int32_t        anisotropy;
		skg_tex_t      tex;
		tex_t          depth_buffer;
	};

	extern void tex_set_surface(tex_t texture, void *native_surface, tex_type_ type, int64_t native_fmt, int32_t width, int32_t height, int32_t surface_count);
	extern void tex_set_options(tex_t texture, tex_sample_ sample, tex_address_ address_mode, int32_t anisotropy_level);

}

extern sk::skg_platform_data_t skg_get_platform_data();