#include "messengermanager.hpp"
#include <String.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Quat.hpp>

using namespace godot;

void MessengerManager::_register_methods() {}


MessengerManager::MessengerManager(){}
MessengerManager::~MessengerManager(){
	delete this->messengerManager;
}

void MessengerManager::_init() {
	this->messengerManager = new StardustXR::MessengerManager(this);
}

void MessengerManager::sendSignal(std::string path, std::string method, flexbuffers::Reference data) {
	Variant variant = flexbufferToVariant(data);
	if (variant.get_type() == Variant::Type::ARRAY) {
		this->get_node(path.c_str()+1)->callv(String(method.c_str()), variant);
	} else {
		Array array;
		array.append(variant);
		this->get_node(path.c_str()+1)->callv(String(method.c_str()), array);
	}
}

std::vector<uint8_t> MessengerManager::executeMethod(std::string path, std::string method, flexbuffers::Reference args) {
	Variant returnVal;
	if (args.IsAnyVector()) {
		returnVal = get_node(path.c_str()+1)->callv(String(method.c_str()), flexbufferToVariant(args));
	} else {
		Array array;
		array.append(flexbufferToVariant(args));
		returnVal = get_node(path.c_str()+1)->callv(String(method.c_str()), array);
	}
	return variantToFlexbuffer(returnVal);
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
		if(vector[0].IsFloat()) {
			switch(vector.size()) {
				case 3: {
					real_t x = vector[0].AsDouble();
					real_t y = vector[1].AsDouble();
					real_t z = vector[2].AsDouble();

					Vector3 vector = Vector3(x, y, z);
					return Variant(vector);
				}
				case 4: {
					real_t x = vector[0].AsDouble();
					real_t y = vector[1].AsDouble();
					real_t z = vector[2].AsDouble();
					real_t w = vector[3].AsDouble();

					Quat quat = Quat(x, y, z, w);
					return Variant(quat);
				}
			}
		}
		for(int i=0; i<vector.size(); ++i) {
			array.append(flexbufferToVariant(vector[i]));
		}
		return Variant(array);
	}

	return Variant();
}

void MessengerManager::flexbufferVariantAdd(flexbuffers::Builder &fbb, Variant variant) {
	switch (variant.get_type()) {
		case Variant::Type::NIL: {
			fbb.Null();
		} break;
		case Variant::Type::BOOL: {
			bool value = variant;
			fbb.Bool(value);
		} break;
		case Variant::Type::INT: {
			int value = variant;
			fbb.Int(value);
		} break;
		case Variant::Type::REAL: {
			double value = variant;
			fbb.Double(value);
		} break;
		case Variant::Type::STRING: {
			String value = variant;
			fbb.String(value.ascii().get_data());
		} break;
		case Variant::Type::VECTOR2: {
			Vector2 vector = variant;
			fbb.TypedVector([&]() {
				fbb.Double(vector.x);
				fbb.Double(vector.y);
			});
		} break;
		case Variant::Type::VECTOR3: {
			Vector3 vector = variant;
			fbb.TypedVector([&]() {
				fbb.Double(vector.x);
				fbb.Double(vector.y);
				fbb.Double(vector.z);
			});
		} break;
		case Variant::Type::QUAT: {
			Quat quat = variant;
			fbb.TypedVector([&]() {
				fbb.Double(quat.x);
				fbb.Double(quat.y);
				fbb.Double(quat.z);
				fbb.Double(quat.w);
			});
		} break;
		// default: {
		// 	fbb.Add(variant);
		// } break;
	}
}

const std::vector<uint8_t> MessengerManager::variantToFlexbuffer(Variant variant) {
	flexbuffers::Builder fbb;
	flexbufferVariantAdd(fbb, variant);
	fbb.Finish();
	return fbb.GetBuffer();
}
