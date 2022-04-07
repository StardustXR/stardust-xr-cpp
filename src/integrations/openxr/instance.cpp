#include "instance.hpp"
#include "../../interfaces/openxr.hpp"
#include <stardustxr/common/flex.hpp>

namespace StardustXRServer {

OpenXRInstance::OpenXRInstance(Client *client,
							   const XrInstanceCreateFlags flags,
							   const std::string appName,
							   const uint32_t appVersion,
							   const std::string engineName,
							   const uint32_t engineVersion,
							   const XrVersion apiVersion) :
				OpenXRObject(client),
				appName(appName),
				appVersion(appVersion),
				engineName(engineName),
				engineVersion(engineVersion),
				apiVersion(apiVersion),
				flags(flags) {

	STARDUSTXR_NODE_METHOD("createSession", &OpenXRInstance::createSession)
}

std::vector<uint8_t> OpenXRInstance::createSession(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
//	XrFormFactor formFactor = (XrFormFactor) data.AsUInt32();
	Node *systemNode = parent->children["system"].get();
	OpenXRSystem *system = static_cast<OpenXRSystem *>(systemNode);

	OpenXRSession *session = new OpenXRSession(client, system);
	sessions.add(session);
	addChild("session", session);
	return FLEX_SINGLE(FLEX_INT(session->getCreateResult()));
}

}
