#pragma once

#include "object.hpp"
#include "session.hpp"

#include <openxr/openxr.h>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>

namespace StardustXRServer {

class OpenXRInstance : public OpenXRObject {
public:
	OpenXRInstance(Client *client,
				   const XrInstanceCreateFlags flags,
				   const std::string appName,
				   const uint32_t appVersion,
				   const std::string engineName,
				   const uint32_t engineVersion,
				   const XrVersion apiVersion);

	const std::string appName;
	const uint32_t appVersion;
	const std::string engineName;
	const uint32_t engineVersion;

	const XrVersion apiVersion;

	std::vector<uint8_t> createSession(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> getViewConfigurations(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	Registry<OpenXRSession> sessions;

private:
	const XrInstanceCreateFlags flags;
};

}
