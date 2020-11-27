#ifndef STARDUSTXRSERVER_FIELDINTERFACE_HPP
#define STARDUSTXRSERVER_FIELDINTERFACE_HPP

#include "../../nodetypes/node.hpp"

namespace StardustXRServer {

class FieldInterface : public Node {
public:
	FieldInterface();
	virtual ~FieldInterface() {}

	std::vector<uint8_t> createSphereField(uint sessionID, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_FIELDINTERFACE_HPP
