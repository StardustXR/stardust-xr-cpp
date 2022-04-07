#pragma once

#include "object.hpp"
#include "system.hpp"

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

private:
	const XrInstanceCreateFlags flags;
};

}
