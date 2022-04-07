#include "instance.hpp"

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
	addChild("system", new OpenXRSystem(client, XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY));
}

}
