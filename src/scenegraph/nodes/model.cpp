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
		children[model.name.c_str()] = modelNode;
	}
	modelQueue.clear();
}

std::vector<uint8_t> ModelInterface::createFromFile(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector = data.AsVector();

	std::string name = vector[0].AsString().str();
	std::string path = vector[1].AsString().str();

	QueuedModel model = { sessionID, name, path };
	modelQueue.push_back(model);

//	if(returnValue) {
//		return StardustXR::FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
//			fbb.Null();
//		});
//	} else {
		return std::vector<uint8_t>();
//	}
}

}
