#include "model.hpp"

namespace StardustXRServer {

ModelInterface::ModelInterface() {
	STARDUSTXR_NODE_METHOD("createFromFile", &ModelInterface::createFromFile)
}

void ModelInterface::update() {
	for(const auto &model : modelQueue) {

		ModelNode *modelNode = new ModelNode();
		modelNode->parent = this;
		modelNode->sessionID = model.id;
		modelNode->model = sk::model_create_file(model.path.c_str());
		modelNode->position = model.position;
		modelNode->rotation = model.rotation;
		modelNode->scale = model.scale;
		modelNode->transformDirty();
		children[model.name.c_str()] = modelNode;
	}
	modelQueue.clear();
}

std::vector<uint8_t> ModelInterface::createFromFile(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector = data.AsVector();

	std::string name = vector[0].AsString().str();
	std::string path = vector[1].AsString().str();

	flexbuffers::TypedVector flexPosition = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexScale = vector[4].AsTypedVector();

	QueuedModel model = {
		sessionID,
		name,
		path,
		vec3 {
			flexPosition[0].AsFloat(),
			flexPosition[1].AsFloat(),
			flexPosition[2].AsFloat()
		},
		quat {
			flexRotation[0].AsFloat(),
			flexRotation[1].AsFloat(),
			flexRotation[2].AsFloat(),
			flexRotation[3].AsFloat()
		},
		vec3 {
			flexScale[0].AsFloat(),
			flexScale[1].AsFloat(),
			flexScale[2].AsFloat()
		}
	};
	modelQueue.push_back(model);

	return std::vector<uint8_t>();
}

}
