#ifndef STARDUSTXRSERVER_INPUTINTERFACE_HPP
#define STARDUSTXRSERVER_INPUTINTERFACE_HPP

#include "../../nodetypes/node.hpp"
#include "../../nodetypes/input/inputhandler.hpp"
#include "../../nodetypes/input/inputmethod.hpp"

namespace StardustXRServer {

class InputInterface : public Node {
public:
	InputInterface();
	~InputInterface();

	std::vector<uint8_t> registerInputHandler(uint sessionID, flexbuffers::Reference data, bool returnValue);

	ThreadSafeList<InputMethod *> inputMethods;
	ThreadSafeList<InputHandler *> inputHandlers;
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_INPUTINTERFACE_HPP
