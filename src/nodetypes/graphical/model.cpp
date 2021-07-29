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

void Model::update() {
	if(queued) {
		queued = false;
		model = model_create_file(modelPath.c_str());
		visible = true;
	}
}

void Model::draw() {
	if(!visible)
		return;

	if(model != nullptr)
		render_add_model(model, worldTransform());
}

std::vector<uint8_t> Model::setMaterialProperty(flexbuffers::Reference data, bool returnValue) {
	if(model == nullptr)
		return std::vector<uint8_t>();

	flexbuffers::Vector vec = data.AsVector();
	uint32_t submeshIndex = vec[0].AsUInt32();
	const char *propertyName = vec[1].AsString().c_str();
	flexbuffers::Reference propertyValue = vec[2];

	material_t mat = model_get_material(model, submeshIndex);

	if(propertyValue.IsFloat()) {
		material_set_float(mat, propertyName, propertyValue.AsFloat());
	} else if (propertyValue.IsTypedVector()) {
		flexbuffers::TypedVector flexColor = propertyValue.AsTypedVector();
		if(flexColor.size() == 4 && flexColor[0].IsFloat()) {
			material_set_color(mat, propertyName, color128{
				flexColor[0].AsFloat(),
				flexColor[1].AsFloat(),
				flexColor[2].AsFloat(),
				flexColor[3].AsFloat()
			});
		}
	} else if (propertyValue.IsString()) {
		material_set_texture(mat, propertyName, tex_create_file(propertyValue.AsString().c_str()));
	}

	return std::vector<uint8_t>();
};

}
