#pragma once

#include "../spatial/alias.hpp"
#include "types/panel.hpp"

namespace StardustXRServer {

class PanelAlias : public SpatialAlias {
public:
	PanelAlias(Client *client, PanelItem *original, std::vector<std::string> methods);

	std::vector<uint8_t> applySurfaceMaterial(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	PanelItem *const panelOriginal;
};

}
