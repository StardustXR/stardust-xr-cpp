#include "instance.hpp"
#include <stardustxr/common/flex.hpp>

using namespace StardustXR;
namespace StardustXRServer {

OpenXRInstance::OpenXRInstance(Client *client,
							   const uint64_t flags,
							   const std::string appName,
							   const uint32_t appVersion,
							   const std::string engineName,
							   const uint32_t engineVersion,
							   const uint64_t apiVersion) :
				Node(client, true),
				appName(appName),
				appVersion(appVersion),
				engineName(engineName),
				engineVersion(engineVersion),
				apiVersion(apiVersion),
				flags(flags) {}

std::vector<uint8_t> OpenXRInstance::createSession(flexbuffers::Reference data, bool returnValue) {
	OpenXRSession *session = new OpenXRSession(client, (OpenXRFormFactor) data.AsUInt32());
	sessionNode = NodeRef(session);
	addChild("session", session);
	return FLEX_SINGLE(FLEX_INT(session->result));
}

OpenXRSession *OpenXRInstance::session() {
	return sessionNode.ptr<OpenXRSession>();
}

}
