#include "drawable.hpp"
#include "../core/client.hpp"

#include "../nodetypes/drawable/model.hpp"

using namespace sk;

namespace StardustXRServer {

std::string DrawableInterface::skytexQueuedPath;
sk::tex_t DrawableInterface::skytex;
std::string DrawableInterface::skylightQueuedPath;
sk::spherical_harmonics_t DrawableInterface::skylight;

DrawableInterface::DrawableInterface(Client *client) : Node(client, false) {
	addChild("model", new Node(client));

	STARDUSTXR_NODE_METHOD("createModelFromFile", &DrawableInterface::createModelFromFile)
	STARDUSTXR_NODE_METHOD("setSkytex", &DrawableInterface::setSkytex)
	STARDUSTXR_NODE_METHOD("setSkylight", &DrawableInterface::setSkylight)
}

void DrawableInterface::updateEnvironment() {
	if(skytexQueuedPath != "") {
		spherical_harmonics_t harmonics;
		tex_release(skytex);
		skytex = tex_create_cubemap_file(skytexQueuedPath.c_str(), true, &harmonics);
		render_set_skytex(skytex);
		skytexQueuedPath = "";
	}
	if(skylightQueuedPath != "") {
		tex_t skylighttex = tex_create_cubemap_file(skylightQueuedPath.c_str(), true, &skylight);
		tex_release(skylighttex);
		render_set_skylight(skylight);
		skylightQueuedPath = "";
	}
}

std::vector<uint8_t> DrawableInterface::setSkytex(Client *, flexbuffers::Reference data, bool) {
	std::string path = data.AsString().str();
	render_enable_skytex(path != "");
	skytexQueuedPath = path;
	return std::vector<uint8_t>();
}

std::vector<uint8_t> DrawableInterface::setSkylight(Client *, flexbuffers::Reference data, bool) {
	std::string path = data.AsString().str();
	skylightQueuedPath = path;
	return std::vector<uint8_t>();
}

std::vector<uint8_t> DrawableInterface::createModelFromFile(Client *, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector = data.AsVector();

	std::string name = vector[0].AsString().str();
	std::string path = vector[1].AsString().str();
	Spatial *spatialParent = this->client->scenegraph.findNode<Spatial>(vector[2].AsString().str());

	flexbuffers::TypedVector flexPosition = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[4].AsTypedVector();
	flexbuffers::TypedVector flexScale = vector[5].AsTypedVector();

	vec3 position {
		flexPosition[0].AsFloat(),
		flexPosition[1].AsFloat(),
		flexPosition[2].AsFloat()
	};
	quat rotation {
		flexRotation[0].AsFloat(),
		flexRotation[1].AsFloat(),
		flexRotation[2].AsFloat(),
		flexRotation[3].AsFloat()
	};
	vec3 scale {
		flexScale[0].AsFloat(),
		flexScale[1].AsFloat(),
		flexScale[2].AsFloat()
	};

	Model *model = new Model(client, path, spatialParent, matrix_trs(position, rotation, scale));
	children["model"]->addChild(name, model);

	return std::vector<uint8_t>();
}

}
