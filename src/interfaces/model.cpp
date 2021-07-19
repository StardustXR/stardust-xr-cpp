#include "model.hpp"

namespace StardustXRServer {

ModelInterface::ModelInterface(Client *client) : Node(client) {
	STARDUSTXR_NODE_METHOD("createFromFile", &ModelInterface::createFromFile)
}

std::vector<uint8_t> ModelInterface::createFromFile(flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector = data.AsVector();

	std::string name = vector[0].AsString().str();
	std::string path = vector[1].AsString().str();

	flexbuffers::TypedVector flexPosition = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexScale = vector[4].AsTypedVector();

	vec3 position {
		flexPosition[0].AsFloat(),
		flexPosition[1].AsFloat(),
		flexPosition[2].AsFloat()
	};
	quat rotation {
		flexRotation[0].AsFloat(),
		flexRotation[1].AsFloat(),
		flexRotation[2].AsFloat(),
		flexRotation[3].AsFloat()
	};
	vec3 scale {
		flexScale[0].AsFloat(),
		flexScale[1].AsFloat(),
		flexScale[2].AsFloat()
	};


	Model *model = new Model(client, path, nullptr, position, rotation, scale);
	this->addChild(name, model);

	return std::vector<uint8_t>();
}

}
