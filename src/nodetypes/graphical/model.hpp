#pragma once

#include "drawablenode.hpp"
#include <string>
#include <stereokit.h>
#include <vector>

namespace StardustXRServer {

class Model : public DrawableNode {
public:
	Model(Client *client, std::string modelPath, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, sk::vec3 scale);
	virtual ~Model();

	void update();
	void draw();

	std::vector<uint8_t> setMaterialProperty(flexbuffers::Reference data, bool returnValue);

protected:
	std::string modelPath;
	sk::model_t model = nullptr;
	bool modelQueued = true;

	struct MaterialProperty {
		std::string name = "";
		uint32_t submeshIndex = 0;

		bool isFloat;
		float floatValue = 0;

		bool isColor;
		color128 colorValue = {0,0,0,0};

		bool isString;
		std::string stringValue = "";
	};

	std::vector<MaterialProperty> queuedProperties;
	std::vector<sk::material_t> modifiedMaterials;
};

}
