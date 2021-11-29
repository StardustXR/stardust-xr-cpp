#pragma once

#if __has_include(<pthread.h>)
	#include <pthread.h>
#endif

#define class class_t
#define static
#include <wlr/xwayland.h>
#undef static
#undef class