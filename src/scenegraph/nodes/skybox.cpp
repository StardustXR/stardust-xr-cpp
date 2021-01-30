#include "skybox.hpp"
#include <stereokit.h>

using namespace sk;

namespace StardustXRServer {

SkyboxInterface::SkyboxInterface() {}

void SkyboxInterface::update() {
	if(skytexEnabled) {
		render_enable_skytex(false);
		skytexEnabled = false;
	}
}

} // namespace StardustXRServer
