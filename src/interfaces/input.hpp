#pragma once

#include "../nodetypes/node.hpp"
#include "../nodetypes/input/inputhandler.hpp"
#include "../nodetypes/input/inputmethod.hpp"
#include <mutex>

namespace StardustXRServer {

class InputInterface : public Node {
public:
	InputInterface(Client *client);
	~InputInterface();

	std::vector<uint8_t> registerInputHandler(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> getInputHandlers(flexbuffers::Reference data, bool returnValue);
	static void processInput();

	static void addInputMethod(InputMethod *method);
	static void addInputHandler(InputHandler *handler);

private:
	static std::mutex inputVectorsMutex;

	static std::vector<InputMethod *> inputMethods;
	static std::vector<InputHandler *> inputHandlers;
	
	static std::vector<uint8_t> CreateInputData(flatbuffers::FlatBufferBuilder &fbb, InputMethod *inputMethod, InputHandler *inputHandler);
	static flatbuffers::FlatBufferBuilder fbb;
};

} // namespace StardustXRServer
