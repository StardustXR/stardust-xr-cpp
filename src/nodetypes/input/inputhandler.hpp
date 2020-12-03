#ifndef STARDUSTXRSERVER_INPUTHANDLER_HPP
#define STARDUSTXRSERVER_INPUTHANDLER_HPP

#include "../node.hpp"
#include "../fields/field.hpp"

namespace StardustXRServer {

class InputHandler : public Node {
public:
	InputHandler();
	virtual ~InputHandler();

	Field *field = nullptr;
	std::string callbackPath;
	std::string callbackMethod;

	std::vector<uint8_t> setCallback(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setField(uint sessionID, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_INPUTHANDLER_HPP
