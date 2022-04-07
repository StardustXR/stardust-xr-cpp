#include "object.hpp"

namespace StardustXRServer {

OpenXRObject::OpenXRObject(Client *client) :
	Node(client, true) {}

XrResult OpenXRObject::getCreateResult() {
	return createResult;
}

} // namespace StardustXRServer
