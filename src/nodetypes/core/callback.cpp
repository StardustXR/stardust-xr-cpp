#include "callback.hpp"
#include "../../core/client.hpp"

namespace StardustXRServer {

void Callback::signal(StardustXR::ArgsConstructor argsConstructor) {
	client->messenger.sendSignal(nodePath, method, argsConstructor);
}
void Callback::signal(std::vector<uint8_t> &flexbuffer) {
	client->messenger.sendSignal(nodePath, method, flexbuffer);
}

void Callback::executeMethod(StardustXR::ArgsConstructor argsConstructor, StardustXR::Callback callback) {
	client->messenger.executeRemoteMethod(nodePath, method, argsConstructor, callback);
}
void Callback::executeMethod(std::vector<uint8_t> &flexbuffer, StardustXR::Callback callback) {
	client->messenger.executeRemoteMethod(nodePath, method, flexbuffer, callback);
}

}
