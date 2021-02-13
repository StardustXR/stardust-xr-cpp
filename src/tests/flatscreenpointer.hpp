#ifndef STARDUSTXRSERVER_FLATSCREENPOINTER_HPP
#define STARDUSTXRSERVER_FLATSCREENPOINTER_HPP

#include "../nodetypes/input/inputmethods/pointer.hpp"

namespace StardustXRServer {

class FlatscreenPointer : public PointerInput {
public:
	FlatscreenPointer();

	void update();
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_FLATSCREENPOINTER_HPP
