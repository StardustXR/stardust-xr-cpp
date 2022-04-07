#pragma once

#include "../../nodetypes/core/node.hpp"
#include "session.hpp"

#include <openxr/openxr.h>
#include <string>
#include <stdint.h>
#include <vector>

namespace StardustXRServer {

class OpenXRSystem : public Node {
public:
	OpenXRSystem(Client *client, XrFormFactor formFactor);

	const XrFormFactor formFactor;

	std::vector<uint8_t> createSession(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> getViewConfigurations(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	OpenXRSession *getSession();

private:
	TypedNodeRef<OpenXRSession> session;
};

} // namespace StardustXRServer
