#include "messengermanager.hpp"

namespace godot {

void MessengerManager::_register_methods() {}

void MessengerManager::_init() {
	this->messengerManager = StardustXR::MessengerManager(this);
}

void MessengerManager::sendSignal(std::string path, std::string method, flexbuffers::Reference data) {
	if (data.IsAnyVector()) {
		this->get_node(path.c_str()+1)->callv(String(method.c_str()), flexbufferToVariant(data));
	}
}

std::vector<uint8_t> MessengerManager::executeMethod(std::string path, std::string method, flexbuffers::Reference args) {
	if (args.IsAnyVector()) {
		this->get_node(path.c_str()+1)->callv(String(method.c_str()), flexbufferToVariant(args));
	}
	return std::vector<uint8_t>();
}

const Variant MessengerManager::flexbufferToVariant(flexbuffers::Reference buffer) {
	if(buffer.IsNull())
		return Variant();
	if(buffer.IsBool())
		return Variant(buffer.AsBool());
	if(buffer.IsInt())
		return Variant(buffer.AsInt64());
	if(buffer.IsUInt())
		return Variant(buffer.AsUInt64());
	if(buffer.IsFloat())
		return Variant(buffer.AsFloat());
	if(buffer.IsString())
		return Variant(buffer.AsString().c_str());
	if(buffer.IsVector()) {
		Array array;
		flexbuffers::Vector vector = buffer.AsVector();
		for(int i=0; i<vector.size(); ++i) {
			array.append(flexbufferToVariant(vector[i]));
		}
		return Variant(array);
	}
	if(buffer.IsTypedVector()) {
		Array array;
		flexbuffers::TypedVector vector = buffer.AsTypedVector();
		for(int i=0; i<vector.size(); ++i) {
			array.append(flexbufferToVariant(vector[i]));
		}
		return Variant(array);
	}
	if(buffer.IsFixedTypedVector()) {
		Array array;
		flexbuffers::FixedTypedVector vector = buffer.AsFixedTypedVector();
		for(int i=0; i<vector.size(); ++i) {
			array.append(flexbufferToVariant(vector[i]));
		}
		return Variant(array);
	}

	return Variant();
}

} // namespace godot
