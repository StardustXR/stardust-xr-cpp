#pragma once

#include "../../nodetypes/core/node.hpp"
#include "../../nodetypes/core/noderef.hpp"
#include "session.hpp"

#include <memory>
#include <string>
#include <stdint.h>
#include <vector>

namespace StardustXRServer {

class OpenXRInstance : public Node {
public:
	OpenXRInstance(Client *client,
				   const uint64_t flags,
				   const std::string appName,
				   const uint32_t appVersion,
				   const std::string engineName,
				   const uint32_t engineVersion,
				   const uint64_t apiVersion);

	const std::string appName;
	const uint32_t appVersion;
	const std::string engineName;
	const uint32_t engineVersion;

	const uint64_t apiVersion;

	std::vector<uint8_t> createSession(flexbuffers::Reference data, bool returnValue);
	OpenXRSession *session();

private:
	NodeRef sessionNode;
	const uint64_t flags;
};

} // namespace StardustXRServer
