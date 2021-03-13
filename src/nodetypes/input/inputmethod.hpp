#pragma once

#include "../node.hpp"
#include "inputhandler.hpp"
#include <stereokit.h>
#include <variant>
#include "inputmethods/flatbuffers/Input_generated.h"

using namespace std;
using namespace sk;
namespace StardustXRServer {

class InputMethod : public Node {
public:
	InputMethod();
	virtual ~InputMethod();

	vector<uint8_t> modifyDatamap(uint sessionID, flexbuffers::Reference data, bool returnValue);

	virtual float distanceTo(InputHandler *handler) = 0;
	virtual InputDataRaw type() = 0;
	virtual flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler) = 0;
	virtual void	 updateInput(InputData *data, SpatialNode *space) = 0;
	virtual vector<uint8_t> serializeDatamap();

protected:
	typedef variant<bool, int32_t, float, sk::vec2, sk::vec3> DatamapVariant;
	map<string, DatamapVariant> datamap;

	DatamapVariant flexRefToVar(flexbuffers::Reference ref);
	void varToMapFlex(flexbuffers::Builder &fbb, std::string key, InputMethod::DatamapVariant value);
};

} // namespace StardustXRServer
