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

	std::vector<uint8_t> getReferenceSpaces(Client *callingClient, flexbuffers::Reference data, bool returnValue);
private:
	OpenXRSystem *system;
};

} // namespace StardustXRServer
