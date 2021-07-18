#pragma once

#include "../inputmethod.hpp"
using namespace sk;
#include "../../../util/raymarch.hpp"

namespace StardustXRServer {

class PointerInput : public InputMethod {
public:
	PointerInput(Client *client);
	virtual ~PointerInput();

	Ray ray;
	float distanceTo(InputHandler *handler);
	InputDataRaw type();
	flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler);
	void updateInput(InputData *data, Spatial *space);
};

} // namespace StardustXRServer
