#include "inputmethod.hpp"
#include <flatbuffers/flatbuffers.h>
#include <stardustxr/common/flex.hpp>

#include "../../interfaces/input.hpp"

namespace StardustXRServer {

Registry<InputMethod> InputMethod::inputMethods;

InputMethod::InputMethod(Client *client, Spatial *spatialParent, pose_t transform, bool rotatable) :
Spatial(client, spatialParent, transform, true, rotatable, false, false) {
	inputMethods.add(this);
}
InputMethod::~InputMethod() {
	inputMethods.remove(this);
}

std::vector<uint8_t> InputMethod::serializeDatamap() {
	return FLEX_SINGLE(
		fbb.Map([&](){
			serializeData(fbb);
		});
	);
}

} // namespace StardustXRServer
