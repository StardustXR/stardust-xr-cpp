#include "environment.hpp"

using namespace sk;

namespace StardustXRServer {

std::string EnvironmentInterface::skytexQueuedPath;
sk::tex_t EnvironmentInterface::skytex;
std::string EnvironmentInterface::skylightQueuedPath;
sk::spherical_harmonics_t EnvironmentInterface::skylight;

EnvironmentInterface::EnvironmentInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("visible", &EnvironmentInterface::visible)
	STARDUSTXR_NODE_METHOD("setSkytex", &EnvironmentInterface::setSkytex)
	STARDUSTXR_NODE_METHOD("setLighting", &EnvironmentInterface::setLighting)
}

void EnvironmentInterface::updateEnvironment() {
	if(skylightQueuedPath != "") {
		tex_create_cubemap_file(skylightQueuedPath.c_str(), true, &skylight);
		render_set_skylight(skylight);
		skylightQueuedPath = "";
	}
	if(skytexQueuedPath != "") {
		skytex = tex_create_cubemap_file(skytexQueuedPath.c_str(), true);
		render_set_skytex(skytex);
		skytexQueuedPath = "";
	}
}

std::vector<uint8_t> EnvironmentInterface::visible(flexbuffers::Reference data, bool) {
	render_enable_skytex(data.AsBool());
	return std::vector<uint8_t>();
}

std::vector<uint8_t> EnvironmentInterface::setSkytex(flexbuffers::Reference data, bool) {
	const char *path = data.AsString().c_str();
	skytexQueuedPath = std::string(path);
	return std::vector<uint8_t>();
}

std::vector<uint8_t> EnvironmentInterface::setLighting(flexbuffers::Reference data, bool) {
	const char *path = data.AsString().c_str();
	skylightQueuedPath = std::string(path);
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
