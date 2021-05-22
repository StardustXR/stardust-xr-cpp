#include "environment.hpp"

using namespace sk;

namespace StardustXRServer {

EnvironmentInterface::EnvironmentInterface() {
	STARDUSTXR_NODE_METHOD("visible", &EnvironmentInterface::visible)
	STARDUSTXR_NODE_METHOD("setSkytex", &EnvironmentInterface::setSkytex)
	STARDUSTXR_NODE_METHOD("setLighting", &EnvironmentInterface::setLighting)
}

void EnvironmentInterface::update() {
	if(skytexDisableLatch) {
		render_enable_skytex(false);
		skytexDisableLatch = false;
	}
}

std::vector<uint8_t> EnvironmentInterface::visible(uint, flexbuffers::Reference data, bool) {
	render_enable_skytex(data.AsBool());
	return std::vector<uint8_t>();
}

std::vector<uint8_t> EnvironmentInterface::setSkytex(uint, flexbuffers::Reference data, bool) {
	const char *path = data.AsString().c_str();
	skytex = tex_create_cubemap_file(path, true);
	render_set_skytex(skytex);
	return std::vector<uint8_t>();
}

std::vector<uint8_t> EnvironmentInterface::setLighting(uint, flexbuffers::Reference data, bool) {
	const char *path = data.AsString().c_str();
	tex_create_cubemap_file(path, true, &lighting);
	render_set_skylight(lighting);
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
