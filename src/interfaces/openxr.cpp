#include "openxr.hpp"
#include "../integrations/openxr/instance.hpp"

#include <stardustxr/common/flex.hpp>

namespace StardustXRServer {

OpenXRInterface::OpenXRInterface(Client *client) :
Node(client) {
	STARDUSTXR_NODE_METHOD("createInstance", &OpenXRInterface::createInstance)
}

std::vector<uint8_t> OpenXRInterface::createInstance(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(instance)
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec          = data.AsVector();
	const uint64_t createFlags           = flexVec[0].AsUInt64();
	const std::string applicationName    = flexVec[1].AsString().str();
	const uint32_t applicationVersion    = flexVec[2].AsUInt32();
	const std::string engineName         = flexVec[3].AsString().str();
	const uint32_t engineVersion         = flexVec[4].AsUInt32();
	const uint64_t apiVersion            = flexVec[5].AsUInt64();

	instance = new OpenXRInstance(client, createFlags, applicationName, applicationVersion, engineName, engineVersion, apiVersion);
	addChild("instance", instance);
	return FLEX_SINGLE(FLEX_INT(instance->getCreateResult()));
}

}
