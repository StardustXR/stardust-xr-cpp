#include "inputmethod.hpp"
#include <flatbuffers/flatbuffers.h>
#include <stardustxr/common/flex.hpp>

#include "../../interfaces/input.hpp"

namespace StardustXRServer {

InputMethod::InputMethod(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, bool rotatable) : Spatial(client, spatialParent, position, rotation, vec3_one, true, rotatable, false, false) {
	const std::lock_guard<std::mutex> lock(InputInterface::inputVectorsMutex);
	InputInterface::inputMethods.push_back(this);
}
InputMethod::~InputMethod() {
	const std::lock_guard<std::mutex> lock(InputInterface::inputVectorsMutex);
	InputInterface::inputMethods.erase(std::remove(InputInterface::inputMethods.begin(), InputInterface::inputMethods.end(), this));
}

std::vector<uint8_t> InputMethod::modifyDatamap(Client *callingClient, flexbuffers::Reference data, bool) {
	flexbuffers::Map map = data.AsMap();
	flexbuffers::TypedVector keys = map.Keys();
	flexbuffers::Vector values = map.Values();

	for(size_t i=0; i<map.size(); ++i) {
		std::string key = keys[i].AsString().str();
		DatamapVariant value = flexRefToVar(values[i]);
		datamap[key] = value;
	}

	return std::vector<uint8_t>();
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
			return sk::vec2 {
				vec[0].AsFloat(),
				vec[1].AsFloat()
			};
		} else if(vec.size() == 3) {
			return sk::vec3 {
				vec[0].AsFloat(),
				vec[1].AsFloat(),
				vec[2].AsFloat()
			};
		}
	} else if (ref.IsString()) {
		return ref.AsString().str();
	}

	return false;
}

void InputMethod::varToMapFlex(flexbuffers::Builder &fbb, std::string key, InputMethod::DatamapVariant value) {
	std::visit([&](auto&& arg) {
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, bool>)
			fbb.Bool(key.c_str(), arg);
		else if constexpr (std::is_same_v<T, int>)
			fbb.Int(key.c_str(), arg);
		else if constexpr (std::is_same_v<T, float>)
			fbb.Float(key.c_str(), arg);
		else if constexpr (std::is_same_v<T, sk::vec2>) {
			sk::vec2 vec = arg;
			fbb.TypedVector(key.c_str(), [&]() {
				fbb.Float(vec.x);
				fbb.Float(vec.y);
			});
		} else if constexpr (std::is_same_v<T, sk::vec3>) {
			sk::vec3 vec = arg;
			fbb.TypedVector(key.c_str(), [&]() {
				fbb.Float(vec.x);
				fbb.Float(vec.y);
				fbb.Float(vec.z);
			});
		} else if constexpr (std::is_same_v<T, std::string>)
			fbb.String(key.c_str(), arg);
	}, value);
}

std::vector<uint8_t> InputMethod::serializeDatamap() {
	return FLEX_SINGLE(
		fbb.Map([&](){
			for(auto const& [mapKey, mapItem] : datamap) {
				varToMapFlex(fbb, mapKey, mapItem);
			}
		});
	);
}

} // namespace StardustXRServer
