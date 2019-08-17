#ifndef OPENXR_META_H
#define OPENXR_META_H

#include "vulkan_meta.h"

#define XR_USE_GRAPHICS_API_VULKAN
#define XR_KHR_vulkan_enable
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_platform_defines.h>
#include <openxr/openxr_reflection.h>

#endif // OPENXR_META_H
