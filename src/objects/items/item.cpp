#include "item.hpp"

namespace StardustXRServer {

Item::Item(Client *client, std::string type, vec3 pos) :
	Spatial(client, nullptr, pos, quat_identity, vec3_one, true, false, false, false),
	type(type) {

}

Item::~Item() {}

}
