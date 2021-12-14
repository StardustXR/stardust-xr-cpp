#include "alias.hpp"
#include "../../core/client.hpp"
#include "../../integrations/wayland/surface.hpp"
#include "../drawable/model.hpp"

namespace StardustXRServer {

PanelAlias::PanelAlias(Client *client, PanelItem *original, std::vector<std::string> methods) :
	SpatialAlias(client, original, methods),
	panelOriginal(original) {
	STARDUSTXR_NODE_METHOD("applySurfaceMaterial", &PanelAlias::applySurfaceMaterial)
}

std::vector<uint8_t> PanelAlias::applySurfaceMaterial(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec      = data.AsVector();
	Model *model				     = this->client->scenegraph.findNode<Model>(flexVec[0].AsString().str());
	uint32_t submeshIndex            = flexVec[0].AsUInt32();

	if(model != nullptr)
		model->replaceMaterial(submeshIndex, panelOriginal->surface->surfaceMatAlphaBlend);
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
