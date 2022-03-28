#pragma once

#include "../nodetypes/core/node.hpp"

#include <stereokit.h>

namespace StardustXRServer {

class DrawableInterface : public Node {
public:
	DrawableInterface(Client *client);

	static void updateEnvironment();

	std::vector<uint8_t> createModelFromFile(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createText(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSkytex(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSkylight(Client *callingClient, flexbuffers::Reference data, bool returnValue);

private:
	static std::string skytexQueuedPath;
	static sk::tex_t skytex;

	static std::string skylightQueuedPath;
	static sk::spherical_harmonics_t skylight;
};

}
