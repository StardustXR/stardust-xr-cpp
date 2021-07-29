#pragma once

#include "drawablenode.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class Model : public DrawableNode {
public:
	Model(Client *client, std::string modelPath, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, sk::vec3 scale);
	virtual ~Model() {}

	void update();
	void draw();

	std::vector<uint8_t> setMaterialProperty(flexbuffers::Reference data, bool returnValue);

	bool queued = true;
	std::string modelPath;
	sk::model_t model = nullptr;
};

}
