#pragma once

#include <string>
#include <vector>

#include "../../nodetypes/spatial/spatial.hpp"

namespace StardustXRServer {

class Item : public Spatial {
public:
	Item(Client *client, std::string type, sk::vec3 pos);
	virtual ~Item();

//	virtual void accept();
	const std::string type;
};

}
