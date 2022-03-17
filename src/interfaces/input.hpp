#pragma once

#include "../nodetypes/core/node.hpp"
#include "../nodetypes/input/inputhandler.hpp"
#include "../nodetypes/input/inputmethod.hpp"
#include <mutex>

namespace StardustXRServer {

class InputInterface : public Node {
public:
	InputInterface(Client *client);

	std::vector<uint8_t> registerInputHandler(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> getInputHandlers(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	static void processInput();

private:
	static std::vector<uint8_t> CreateInputData(flatbuffers::FlatBufferBuilder &fbb, InputMethod *inputMethod, InputHandler *inputHandler);
	static flatbuffers::FlatBufferBuilder fbb;
};

} // namespace StardustXRServer
