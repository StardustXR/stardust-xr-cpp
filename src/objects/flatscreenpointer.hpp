#pragma once

#include "../nodetypes/input/inputmethods/pointer.hpp"

namespace StardustXRServer {

class FlatscreenPointer : public PointerInput {
public:
	FlatscreenPointer(Client *client);

	void update();
};

} // namespace StardustXRServer
