#pragma once

#include "../../nodetypes/core/node.hpp"
#include <openxr/openxr.h>

namespace StardustXRServer {

class OpenXRObject : public Node {
public:
	OpenXRObject(Client *client);

	XrResult getCreateResult();
protected:
	XrResult createResult = XR_SUCCESS;
};

} // namespace StardustXRServer
