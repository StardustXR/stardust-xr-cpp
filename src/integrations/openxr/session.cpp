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
	Node *spaces = new Node(client, false);
	addChild("spaces", spaces);

	Spatial *localSpace = new Spatial(client, client->scenegraph.root.worldTransform());
	spaces->addChild("XR_REFERENCE_SPACE_TYPE_LOCAL", localSpace);

	Spatial *viewSpace = new Spatial(client, hmd, matrix_identity, false, false, false, false);
	spaces->addChild("XR_REFERENCE_SPACE_TYPE_VIEW", viewSpace);

	Spatial *stageSpace = new Spatial(client, matrix_identity);
	spaces->addChild("XR_REFERENCE_SPACE_TYPE_STAGE", stageSpace);
}

}
