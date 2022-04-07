#pragma once

#include "object.hpp"
#include <string>
#include <stdint.h>
#include <vector>

namespace StardustXRServer {

class OpenXRSession : public OpenXRObject {
public:
	OpenXRSession(Client *client);
};

} // namespace StardustXRServer
