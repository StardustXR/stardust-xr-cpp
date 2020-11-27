#ifndef STARDUSTXR_MODEL_INTERFACE_H
#define STARDUSTXR_MODEL_INTERFACE_H

#include "../../nodetypes/graphical/modelnode.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class ModelInterface : public Node {
public:
	ModelInterface();
	virtual ~ModelInterface() {}

	std::vector<uint8_t> createFromFile(uint sessionID, flexbuffers::Reference data, bool returnValue);
};

}

#endif // STARDUSTXR_MODEL_INTERFACE_H
