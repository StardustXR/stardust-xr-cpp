#pragma once

#include "../inputmethod.hpp"
using namespace sk;
#include "../../../util/raymarch.hpp"

namespace StardustXRServer {

class PointerInput : public InputMethod, public SpatialNode {
public:
	PointerInput();
	virtual ~PointerInput();

	Ray ray;
	float distanceTo(InputHandler *handler);
	InputDataRaw type();
	flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler);
	void updateInput(InputData *data, SpatialNode *space);
};

} // namespace StardustXRServer
