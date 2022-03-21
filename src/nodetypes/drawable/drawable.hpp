#pragma once

#include "../spatial/spatial.hpp"
#include <vector>

namespace StardustXRServer {

class Drawable : public Spatial {
public:
	Drawable(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, sk::vec3 scale, bool translatable, bool rotatable, bool scalable);
	virtual ~Drawable();
	virtual void draw() {}

	static void drawAll();

protected:
	static Registry<Drawable> drawables;
};

} //namespace StardustXRServer
