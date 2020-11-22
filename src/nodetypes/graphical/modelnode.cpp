#include "modelnode.hpp"
using namespace sk;

namespace StardustXRServer {

ModelNode::ModelNode() {}

void ModelNode::draw() {
	if(model != nullptr)
		render_add_model(model, matrix_trs(vec3_forward, quat_identity, vec3_one));
}

}
