#pragma once

#include "object.hpp"
#include "system.hpp"
#include <string>
#include <stdint.h>
#include <vector>

namespace StardustXRServer {

class OpenXRSession : public OpenXRObject {
public:
	OpenXRSession(Client *client, OpenXRSystem *system);

private:
	OpenXRSystem *system;
};

} // namespace StardustXRServer
