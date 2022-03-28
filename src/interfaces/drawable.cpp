#include "drawable.hpp"
#include "../core/client.hpp"
#include "../nodetypes/drawable/model.hpp"
#include "../util/flex.hpp"

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
	flexbuffers::Vector flexVec = data.AsVector();

	std::string name            = flexVec[0].AsString().str();
	std::string path            = flexVec[1].AsString().str();
	Spatial *spatialParent      = this->client->scenegraph.findNode<Spatial>(flexVec[2].AsString().str());
	matrix transform            = FlexToSKTransform(flexVec[3].AsTypedVector(), flexVec[4].AsTypedVector(), flexVec[5].AsTypedVector());

	Model *model = new Model(client, path, spatialParent, transform);
	children["model"]->addChild(name, model);

	return std::vector<uint8_t>();
}

}
