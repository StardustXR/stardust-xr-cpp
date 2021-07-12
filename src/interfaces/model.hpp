#pragma once

#include "../nodetypes/graphical/model.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class ModelInterface : public Node {
public:
	ModelInterface(Client *client);
	virtual ~ModelInterface() {}

	std::vector<uint8_t> createFromFile(uint sessionID, flexbuffers::Reference data, bool returnValue);
};

}
