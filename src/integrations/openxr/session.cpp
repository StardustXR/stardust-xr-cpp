#include "session.hpp"
#include <openxr/openxr.h>

namespace StardustXRServer {

OpenXRSession::OpenXRSession(Client *client, OpenXRSystem *system) :
OpenXRObject(client),
system(system) {}

}
