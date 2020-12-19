#include "inputmethod.hpp"
#include "flatbuffers/flexbuffers.h"

namespace StardustXRServer {

InputMethod::InputMethod() {}
InputMethod::~InputMethod() {}

vector<uint8_t> InputMethod::modifyDatamap(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Map map = data.AsMap();
	flexbuffers::TypedVector keys = map.Keys();
	flexbuffers::Vector values = map.Values();

	if(sessionID != this->sessionID)
		goto RETURN;

	for(size_t i=0; i<map.size(); ++i) {
		std::string key = keys[i].AsString().str();
		DatamapVariant value = flexRefToVar(values[i]);
		datamap[key] = value;
	}

	RETURN:
	return vector<uint8_t>();
}

InputMethod::DatamapVariant InputMethod::flexRefToVar(flexbuffers::Reference ref) {
	if(ref.IsBool())
		return ref.AsBool();
	else if (ref.IsIntOrUint())
		return ref.AsInt32();
	else if (ref.IsFloat())
		return ref.AsFloat();
	else if (ref.IsTypedVector()) {
		flexbuffers::TypedVector vec = ref.AsTypedVector();
		if(vec.size() == 2) {
			return vec2 {
				vec[0].AsFloat(),
				vec[1].AsFloat()
			};
		} else if(vec.size() == 3) {
			return vec3 {
				vec[0].AsFloat(),
				vec[1].AsFloat(),
				vec[2].AsFloat()
			};
		}
	}

	return false;
}

void InputMethod::varToFlex(flexbuffers::Builder &fbb, InputMethod::DatamapVariant var) {
	std::visit([&](auto&& arg) {
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, bool>)
			fbb.Bool(arg);
		else if constexpr (std::is_same_v<T, int>)
			fbb.Int(arg);
		else if constexpr (std::is_same_v<T, float>)
			fbb.Float(arg);
		else if constexpr (std::is_same_v<T, vec2>) {
			vec2 vec = arg;
			fbb.TypedVector([&]() {
				fbb.Float(vec.x);
				fbb.Float(vec.y);
			});
		} else if constexpr (std::is_same_v<T, vec3>) {
			vec3 vec = arg;
			fbb.TypedVector([&]() {
				fbb.Float(vec.x);
				fbb.Float(vec.y);
				fbb.Float(vec.z);
			});
		}
	}, var);
}

} // namespace StardustXRServer
