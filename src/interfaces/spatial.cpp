#include "spatial.hpp"
#include "../nodetypes/fields/boxfield.hpp"
#include "../nodetypes/fields/spherefield.hpp"
#include "../core/client.hpp"

using namespace sk;
using namespace std;

namespace StardustXRServer {

SpatialFactory::SpatialFactory(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("createSpatial", &SpatialFactory::createSpatial)
	addChild("spatial", new Node(client));
}

std::vector<uint8_t> SpatialFactory::createSpatial(flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector            = data.AsVector();
	string name                           = vector[0].AsString().str();
	Spatial *spatialParent                = this->client->scenegraph.findNode<Spatial>(vector[1].AsString().str());
	flexbuffers::TypedVector flexPosition = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexScale    = vector[4].AsTypedVector();
	bool translatable                     = vector[5].AsBool();
	bool rotatable                        = vector[6].AsBool();
	bool scalable                         = vector[7].AsBool();

	vec3 position = {
		flexPosition[0].AsFloat(),
		flexPosition[1].AsFloat(),
		flexPosition[2].AsFloat()
	};
	quat rotation = {
		flexRotation[0].AsFloat(),
		flexRotation[1].AsFloat(),
		flexRotation[2].AsFloat(),
		flexRotation[3].AsFloat()
	};
	vec3 scale = {
		flexScale[0].AsFloat(),
		flexScale[1].AsFloat(),
		flexScale[2].AsFloat()
	};

	Spatial *spatial = new Spatial(client, spatialParent, position, rotation, scale, translatable, rotatable, scalable);
	children["spatial"]->addChild(name, spatial);

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
