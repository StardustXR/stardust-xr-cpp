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
	InputMethod(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, bool rotatable);
	virtual ~InputMethod();

	std::vector<uint8_t> modifyDatamap(flexbuffers::Reference data, bool returnValue);

	virtual float distanceTo(InputHandler *handler) = 0;
	virtual InputDataRaw type() = 0;
	virtual flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler) = 0;
	virtual void	 updateInput(InputData *data, Spatial *space) = 0;
	virtual std::vector<uint8_t> serializeDatamap();

	typedef std::variant<bool, int32_t, float, sk::vec2, sk::vec3, std::string> DatamapVariant;
	std::map<std::string, DatamapVariant> datamap;

protected:
	DatamapVariant flexRefToVar(flexbuffers::Reference ref);
	void varToMapFlex(flexbuffers::Builder &fbb, std::string key, InputMethod::DatamapVariant value);
};

} // namespace StardustXRServer
