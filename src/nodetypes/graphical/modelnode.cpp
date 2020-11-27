#include "modelnode.hpp"
using namespace sk;

namespace StardustXRServer {

ModelNode::ModelNode() {}

void ModelNode::update() {
	if(queued) {
		queued = false;
		model = sk::model_create_file(modelPath.c_str());
		visible = true;
	}
}

void ModelNode::draw() {
	if(!ready || !visible)
		return;

	if(model != nullptr)
		render_add_model(model, worldTransform());
}

}
