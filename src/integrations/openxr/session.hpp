#pragma once

#include "../../nodetypes/core/node.hpp"

#include <string>
#include <stdint.h>
#include <vector>

namespace StardustXRServer {

enum OpenXRFormFactor {
	XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY = 1,
	XR_FORM_FACTOR_HANDHELD_DISPLAY = 2,
	XR_FORM_FACTOR_MAX_ENUM = 0x7FFFFFFF
};

class OpenXRSession : public Node {
public:
	OpenXRSession(Client *client, OpenXRFormFactor formFactor);

	int64_t result = 0;
	const OpenXRFormFactor formFactor;
};

} // namespace StardustXRServer
