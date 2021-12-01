#pragma once

#include "../spatial/alias.hpp"
#include "panel.hpp"

namespace StardustXRServer {

class PanelAlias : public SpatialAlias {
public:
	PanelAlias(Client *client, PanelItem *original, std::vector<std::string> methods);

	std::vector<uint8_t> applySurfaceMaterial(flexbuffers::Reference data, bool returnValue);

	PanelItem *const panelOriginal;
};

}
