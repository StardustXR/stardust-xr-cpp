#pragma once

#include <stereokit.h>
#include "../nodetypes/node.hpp"

namespace StardustXRServer {

class EnvironmentInterface : public Node {
public:
	EnvironmentInterface(Client *client);
	virtual ~EnvironmentInterface() {}

	static void updateEnvironment();

	std::vector<uint8_t> visible(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSkytex(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setLighting(uint sessionID, flexbuffers::Reference data, bool returnValue);

private:
	static std::string skytexQueuedPath;
	static sk::tex_t skytex;

	static std::string skylightQueuedPath;
	static sk::spherical_harmonics_t skylight;
};

} // namespace StardustXRServer
