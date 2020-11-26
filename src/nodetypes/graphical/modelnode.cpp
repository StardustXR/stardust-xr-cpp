#include "modelnode.hpp"
using namespace sk;

namespace StardustXRServer {

ModelNode::ModelNode() {}

void ModelNode::draw() {
	if(transformMatrixDirty)
		transform = matrix_trs(position, orientation, scale);

	if(model != nullptr)
		render_add_model(model, transform);
}

}
