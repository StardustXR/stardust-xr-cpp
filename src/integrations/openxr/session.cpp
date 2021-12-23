#include "session.hpp"
#include <openxr/openxr.h>

namespace StardustXRServer {

OpenXRSession::OpenXRSession(Client *client, OpenXRFormFactor formFactor) :
	Node(client, true),
	formFactor(formFactor) {
	if(formFactor != OpenXRFormFactor::XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY)
		result = XR_ERROR_SYSTEM_INVALID;
}

}
