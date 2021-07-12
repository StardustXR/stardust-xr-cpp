#pragma once

#include "../nodetypes/node.hpp"
#include "../nodetypes/input/inputhandler.hpp"
#include "../nodetypes/input/inputmethod.hpp"
#include <stardustxr/util/threadsafelist.hpp>

namespace StardustXRServer {

class InputInterface : public Node {
public:
	InputInterface(Client *client);
	~InputInterface();
	void handleMessengerDeletion(uint sessionID);

	std::vector<uint8_t> registerInputHandler(uint sessionID, flexbuffers::Reference data, bool returnValue);
	static void processInput();

	static ThreadSafeList<InputMethod *> inputMethods;
	static ThreadSafeList<InputHandler *> inputHandlers;

private:
	static std::vector<uint8_t> CreateInputData(flatbuffers::FlatBufferBuilder &fbb, InputMethod *inputMethod, InputHandler *inputHandler);
	static flatbuffers::FlatBufferBuilder fbb;
};

} // namespace StardustXRServer
