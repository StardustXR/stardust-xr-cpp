#pragma once

#include "../spatial/spatial.hpp"
#include <vector>

namespace StardustXRServer {

class Drawable : public Spatial {
public:
	Drawable(Client *client, Spatial *spatialParent, matrix transform, bool translatable, bool rotatable, bool scalable);
	virtual ~Drawable();
	virtual void draw() {}

	static void drawAll();

protected:
	static Registry<Drawable> drawables;
};

} //namespace StardustXRServer
