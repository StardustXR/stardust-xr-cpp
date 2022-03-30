#pragma once

#include "../spatial/spatial.hpp"
#include "inputhandler.hpp"
#include <stardustxr/common/flatbuffers/Input.hpp>
#include <stereokit.h>
#include <variant>

using namespace StardustXR;
using namespace sk;
namespace StardustXRServer {

class InputMethod : public Spatial {
public:
	InputMethod(Client *client, Spatial *spatialParent, sk::pose_t transform, bool rotatable);
	static Registry<InputMethod> inputMethods;
	virtual ~InputMethod();

	virtual float distanceTo(InputHandler *handler) = 0;
	virtual InputDataRaw type() = 0;
	virtual flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler) = 0;
	virtual void updateInput(InputData *data, InputHandler *handler) = 0;

	std::vector<uint8_t> serializeDatamap();
	virtual void serializeData(flexbuffers::Builder &fbb) = 0;
};

} // namespace StardustXRServer
