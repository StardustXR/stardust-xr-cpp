#pragma once

#include "drawablenode.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class Model : public DrawableNode {
public:
	Model(Client *client);
	virtual ~Model() {}

	void update();
	void draw();

	bool queued = true;
	std::string modelPath;
	sk::model_t model = nullptr;
};

}
