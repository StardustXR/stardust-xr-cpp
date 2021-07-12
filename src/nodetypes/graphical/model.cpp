#include "model.hpp"
using namespace sk;

namespace StardustXRServer {

Model::Model(Client *client) : DrawableNode(client) {}

void Model::update() {
	if(queued) {
		queued = false;
		model = model_create_file(modelPath.c_str());
		visible = true;
	}
}

void Model::draw() {
	if(!ready || !visible)
		return;

	if(model != nullptr)
		render_add_model(model, worldTransform());
}

}
