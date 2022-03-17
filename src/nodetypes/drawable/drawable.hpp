#pragma once

#include "../spatial/spatial.hpp"
#include <vector>

namespace StardustXRServer {

class Drawable : public Spatial {
public:
	Drawable(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, sk::vec3 scale, bool translatable, bool rotatable, bool scalable) : Spatial(client, spatialParent, position, rotation, scale, translatable, rotatable, scalable, false) {}
	virtual void draw() {}
};

} //namespace StardustXRServer
