#include "system.hpp"
#include <stardustxr/common/flex.hpp>
#include "stereokit.h"

using namespace sk;

namespace StardustXRServer {

OpenXRSystem::OpenXRSystem(Client *client, XrFormFactor formFactor) :
Node(client, false),
formFactor(formFactor) {
	STARDUSTXR_NODE_METHOD("getViewConfigurations", &OpenXRSystem::getViewConfigurations)
}

std::vector<uint8_t> OpenXRSystem::getViewConfigurations(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	system_info_t skSys = sk_system_info();
	// For testing purposes, make this false no matter what
//	bool flatscreen = sk_active_display_mode() == display_mode_flatscreen;
	bool flatscreen = false;
	return FLEX(
		for(uint i=0; i<(flatscreen ? 1 : 2); i++) {
			FLEX_VEC(
				FLEX_UINT(skSys.display_width)
				FLEX_UINT(skSys.display_width)
				FLEX_UINT(skSys.display_height)
				FLEX_UINT(skSys.display_height)
			)
		}
	);
}


}
