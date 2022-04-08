#pragma once

#include "../nodetypes/spatial/spatial.hpp"

namespace StardustXRServer {

class HMD : public Spatial {
public:
	HMD(Client *client);

	void update();
};

}
