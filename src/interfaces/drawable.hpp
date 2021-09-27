#pragma once

#include "../nodetypes/graphical/model.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class DrawableInterface : public Node {
public:
	DrawableInterface(Client *client);

	std::vector<uint8_t> createModelFromFile(flexbuffers::Reference data, bool returnValue);
};

}
