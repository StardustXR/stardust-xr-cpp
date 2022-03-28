#include "drawable.hpp"

namespace StardustXRServer {

Registry<Drawable> Drawable::drawables;

Drawable::Drawable(Client *client, Spatial *spatialParent, matrix transform, bool translatable, bool rotatable, bool scalable) :
	Spatial(client, spatialParent, transform, translatable, rotatable, scalable, false) {

	drawables.add(this);
}

Drawable::~Drawable() {
	drawables.remove(this);
}

void Drawable::drawAll() {
	for(Drawable *drawable : drawables.list()) {
		drawable->draw();
	}
}

}
