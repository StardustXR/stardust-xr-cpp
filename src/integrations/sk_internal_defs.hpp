#pragma once

#include "stereokit.h"

namespace sk {

	typedef struct skg_platform_data_t {
		void *_egl_display;
		void *_egl_config;
		void *_egl_context;
	} skg_platform_data_t;
}

extern sk::skg_platform_data_t skg_get_platform_data();
