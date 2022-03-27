#include "callback.hpp"
#include "../../core/client.hpp"

namespace StardustXRServer {

void Callback::signal(StardustXR::ArgsConstructor argsConstructor) {
	if(!client->disconnected)
		client->messenger.sendSignal(nodePath, method, argsConstructor);
}
void Callback::signal(std::vector<uint8_t> &flexbuffer) {
	if(!client->disconnected)
		client->messenger.sendSignal(nodePath, method, flexbuffer);
}

void Callback::executeMethod(StardustXR::ArgsConstructor argsConstructor, StardustXR::Callback callback) {
	if(!client->disconnected)
		client->messenger.executeRemoteMethod(nodePath, method, argsConstructor, callback);
}
void Callback::executeMethod(std::vector<uint8_t> &flexbuffer, StardustXR::Callback callback) {
	if(!client->disconnected)
		client->messenger.executeRemoteMethod(nodePath, method, flexbuffer, callback);
}

}
