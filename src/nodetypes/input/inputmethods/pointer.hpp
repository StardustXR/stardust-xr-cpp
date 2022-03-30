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
	RayMarchResult march(Field *field);
	float distanceTo(InputHandler *handler);
	InputDataRaw type();
	flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler);
	void updateInput(InputData *data, InputHandler *handler);

	virtual void serializeData(flexbuffers::Builder &fbb);

private:
	float deepestPointDistance;
};

} // namespace StardustXRServer
