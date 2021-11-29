#pragma once

#include "../core/alias.hpp"
#include "spatial.hpp"

namespace StardustXRServer {

class SpatialAlias : public Alias {
public:
	SpatialAlias(Client *client, Spatial *original, std::vector<std::string> methods);

	std::vector<uint8_t> setSpatialParentFlex        (flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSpatialParentInPlaceFlex (flexbuffers::Reference data, bool returnValue);

	Spatial *const spatialOriginal;
};

}
