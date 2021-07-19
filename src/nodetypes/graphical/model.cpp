#include "model.hpp"
using namespace sk;

namespace StardustXRServer {

Model::Model(Client *client, std::string modelPath, Spatial *spatialParent, vec3 position, quat rotation, vec3 scale) : DrawableNode(client, spatialParent, position, rotation, scale, true, true, true) {
	this->modelPath = modelPath;
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

}
