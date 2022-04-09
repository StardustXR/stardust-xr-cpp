#include "session.hpp"
#include "../../nodetypes/spatial/spatial.hpp"
#include "../../core/client.hpp"
#include "../../globals.hpp"
#include <openxr/openxr.h>

using namespace sk;

namespace StardustXRServer {

OpenXRSession::OpenXRSession(Client *client, OpenXRSystem *system) :
OpenXRObject(client),
system(system) {
	STARDUSTXR_NODE_METHOD("getReferenceSpaces", &OpenXRSession::getReferenceSpaces)

	Spatial *localSpace = new Spatial(client, nullptr, client->scenegraph.root.worldTransform(), true, true, false, false);
	Spatial *viewSpace = new Spatial(client, hmd, matrix_identity, false, false, false, false);
	Spatial *stageSpace = new Spatial(client, nullptr, matrix_identity, true, true, false, false);

	addChild("referenceSpace"+std::to_string((uint) XR_REFERENCE_SPACE_TYPE_LOCAL), localSpace);
	addChild("referenceSpace"+std::to_string((uint) XR_REFERENCE_SPACE_TYPE_VIEW),  viewSpace);
	addChild("referenceSpace"+std::to_string((uint) XR_REFERENCE_SPACE_TYPE_STAGE), stageSpace);
}

std::vector<uint8_t> OpenXRSession::getReferenceSpaces(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return FLEX_SINGLE(
		FLEX_TYPED_VEC(
			FLEX_UINT(XR_REFERENCE_SPACE_TYPE_VIEW)
			FLEX_UINT(XR_REFERENCE_SPACE_TYPE_LOCAL)
			FLEX_UINT(XR_REFERENCE_SPACE_TYPE_STAGE)
		)
	);
}

}
