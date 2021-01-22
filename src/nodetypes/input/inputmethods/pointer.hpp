#ifndef STARDUSTXRSERVER_POINTERINPUT_HPP
#define STARDUSTXRSERVER_POINTERINPUT_HPP

#include "../inputmethod.hpp"

namespace StardustXRServer {

class PointerInput : public InputMethod {
public:
	PointerInput();
	virtual ~PointerInput();

	float distanceTo(InputHandler *handler);
	vector<uint8_t> serialize(float distance);
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_POINTERINPUT_HPP
