#ifndef MESSENGERMANAGER_H
#define MESSENGERMANAGER_H

#include <Godot.hpp>
#include <Node.hpp>
#include <Variant.hpp>
#include <stardustxr/scenegraph.hpp>
#include <stardustxr/server/messengermanager.hpp>
#include <flatbuffers/util.h>
#include <unistd.h>

namespace godot {

class MessengerManager : public Node, public StardustXR::Scenegraph {
	GODOT_CLASS(MessengerManager, Node);

public:
	static void _register_methods();

	MessengerManager();
	~MessengerManager();

	void _init();

	void send_signal(std::string path, std::string method, Variant data);
	void sendSignal(std::string path, std::string method, flexbuffers::Reference data);
	std::vector<uint8_t> executeMethod(std::string path, std::string method, flexbuffers::Reference args);

	private:
	StardustXR::MessengerManager *messengerManager;
		const Variant flexbufferToVariant(flexbuffers::Reference buffer);
		const std::vector<uint8_t> variantToFlexbuffer(Variant variant);
	void flexbufferVariantAdd(flexbuffers::Builder &fbb, Variant variant);
};

} // namespace godot

#endif
