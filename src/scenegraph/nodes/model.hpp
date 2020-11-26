#ifndef STARDUSTXR_MODEL_INTERFACE_H
#define STARDUSTXR_MODEL_INTERFACE_H

#include "../../nodetypes/graphical/modelnode.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class ModelInterface : public Node {
public:
	ModelInterface();
	virtual ~ModelInterface() {}

	virtual void update();

	std::vector<uint8_t> createFromFile(uint sessionID, flexbuffers::Reference data, bool returnValue);
protected:
	typedef struct {
		uint id;
		std::string name;
		std::string path;
	} QueuedModel;

	std::vector<QueuedModel> modelQueue;
};

}

#endif // STARDUSTXR_MODEL_INTERFACE_H
