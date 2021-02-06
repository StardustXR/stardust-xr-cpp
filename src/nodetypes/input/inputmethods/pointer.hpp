#ifndef STARDUSTXRSERVER_POINTERINPUT_HPP
#define STARDUSTXRSERVER_POINTERINPUT_HPP

#include "../inputmethod.hpp"

namespace StardustXRServer {

class PointerInput : public InputMethod, public SpatialNode {
public:
	PointerInput();
	virtual ~PointerInput();

	float distanceTo(InputHandler *handler);
	InputDataRaw type();
	flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, SpatialNode *space);
	void updateInput(InputData *data, SpatialNode *space);
	vector<uint8_t> serializeDatamap();
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_POINTERINPUT_HPP
