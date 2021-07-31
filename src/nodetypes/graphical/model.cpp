#include "model.hpp"
#include "stereokit.h"
#include <flatbuffers/flexbuffers.h>
#include <string>
#include <vector>
using namespace sk;

namespace StardustXRServer {

Model::Model(Client *client, std::string modelPath, Spatial *spatialParent, vec3 position, quat rotation, vec3 scale) : DrawableNode(client, spatialParent, position, rotation, scale, true, true, true) {
	this->modelPath = modelPath;
	STARDUSTXR_NODE_METHOD("setMaterialProperty", &Model::setMaterialProperty);
}

Model::~Model() {
	model_release(model);
}

void Model::update() {
	if(modelQueued) {
		modelQueued = false;
		model_t original = model_create_file(modelPath.c_str());
		model = model_copy(original);
		model_release(original);
		visible = true;
	}

	for(MaterialProperty &prop : queuedProperties) {
		material_t originalMat = model_get_material(model, prop.submeshIndex);
		auto modifiedMat = std::find(modifiedMaterials.begin(), modifiedMaterials.end(), originalMat);
		material_t mat;
		if(modifiedMat != modifiedMaterials.end()) {
			mat = *modifiedMat;
		} else {
			mat = material_copy(originalMat);
			model_set_material(model, prop.submeshIndex, mat);
		}

		if(prop.isFloat)
			material_set_float(mat, prop.name.c_str(), prop.floatValue);
		if(prop.isColor)
			material_set_color(mat, prop.name.c_str(), prop.colorValue);
		if(prop.isString) {
			tex_t tex = tex_create_file(prop.stringValue.c_str());
			if(material_set_texture(mat, prop.name.c_str(), tex))
				tex_release(tex);
		}
	}
	queuedProperties.clear();
}

void Model::draw() {
	if(!visible)
		return;

	if(model != nullptr)
		render_add_model(model, worldTransform());
}

std::vector<uint8_t> Model::setMaterialProperty(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vec = data.AsVector();
	uint32_t submeshIndex = vec[0].AsUInt32();
	const char *propertyName = vec[1].AsString().c_str();
	flexbuffers::Reference propertyValue = vec[2];

	MaterialProperty prop = {};
	prop.submeshIndex = submeshIndex;
	prop.name = propertyName;

	prop.isFloat = propertyValue.IsFloat();
	prop.isColor = propertyValue.IsTypedVector();
	prop.isString = propertyValue.IsString();

	if(prop.isFloat) {
		prop.floatValue = propertyValue.AsFloat();
	} else if (prop.isColor) {
		flexbuffers::TypedVector flexColor = propertyValue.AsTypedVector();
		if(flexColor.size() == 4 && flexColor[0].IsFloat()) {
			prop.colorValue = color128{
				flexColor[0].AsFloat(),
				flexColor[1].AsFloat(),
				flexColor[2].AsFloat(),
				flexColor[3].AsFloat()
			};
		} else {
			return std::vector<uint8_t>();
		}
	} else if (prop.isString) {
		prop.stringValue = propertyValue.AsString().str();
	}

	queuedProperties.push_back(prop);

	return std::vector<uint8_t>();
};

}
