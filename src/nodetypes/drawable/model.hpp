#pragma once

#include "drawable.hpp"
#include <mutex>
#include <string>
#include <stereokit.h>
#include <vector>

namespace StardustXRServer {

class Model : public Drawable {
public:
	Model(Client *client, std::string modelPath, Spatial *spatialParent, matrix transform);
	virtual ~Model();

	void draw();

	std::vector<uint8_t> setMaterialProperty(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	void replaceMaterial(uint32_t submeshIndex, sk::material_t mat);

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

	struct MaterialReplacement {
		uint32_t submeshIndex = 0;
		sk::material_t mat;
	};

	std::vector<MaterialReplacement> queuedMaterialReplacements;
	std::vector<MaterialProperty> queuedProperties;
	std::vector<sk::material_t> modifiedMaterials;

    std::mutex queuedPropertiesMutex;
};

}
