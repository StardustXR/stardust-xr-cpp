#ifndef MESSENGERMANAGER_H
#define MESSENGERMANAGER_H

#include <Godot.hpp>
#include <Node.hpp>
#include <Variant.hpp>
#include <stardustxr/server/scenegraph.hpp>
#include <stardustxr/server/messengermanager.hpp>
#include <flatbuffers/util.h>
#include <unistd.h>

namespace godot {

class MessengerManager : public Node, public StardustXR::ServerScenegraph {
	GODOT_CLASS(MessengerManager, Node);

public:
	static void _register_methods();

	MessengerManager();
	~MessengerManager();

	void _init();

	void send_signal(int sessionID, String path, String method, Variant data);
	void execute_remote_method(int clientID, String remotePath, String remoteMethod, Variant args, String callbackPath, String callbackMethod);
	void sendSignal(int sessionID, std::string path, std::string method, flexbuffers::Reference data);
	std::vector<uint8_t> executeMethod(int sessionID, std::string path, std::string method, flexbuffers::Reference args);

private:
	StardustXR::MessengerManager *messengerManager;

	const Variant flexbufferToVariant(flexbuffers::Reference buffer);

	std::vector<uint8_t> variantToFlexbuffer(Variant variant);
	void flexbufferVariantAdd(flexbuffers::Builder &fbb, Variant variant);

	std::vector<uint8_t> nodeMethodExecute(int sessionID, std::string path, std::string method, flexbuffers::Reference args, bool returnValue);
};

} // namespace godot

#endif
