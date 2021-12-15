#include "panel.hpp"
#include "../../core/client.hpp"
#include "../drawable/model.hpp"
#include "../../integrations/wayland/surface.hpp"
#include "alias.hpp"

namespace StardustXRServer {

Item::TypeInfo PanelItem::itemTypeInfo = {
	"panel",
	{
		"applySurfaceMaterial",
		"getData",
		"close",
	}
};

PanelItem::PanelItem(Client *client, Surface *surface, pose_t pose) :
	Item(client, itemTypeInfo, pose),
	surface(surface) {
	STARDUSTXR_NODE_METHOD("applySurfaceMaterial", &PanelItem::applySurfaceMaterial)
	STARDUSTXR_NODE_METHOD("close", &PanelItem::close)
}
PanelItem::~PanelItem() {}

void PanelItem::serializeData(flexbuffers::Builder &fbb) {
	fbb.Vector([&]() {
		fbb.UInt(surface->width);
		fbb.UInt(surface->height);
		fbb.UInt(surface->density);
	});
}

Alias *PanelItem::makeAlias(Client *client) {
	return new PanelAlias(client, this, itemTypeInfo.aliasedMethods);
}

std::vector<uint8_t> PanelItem::applySurfaceMaterial(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec      = data.AsVector();
	Model *model				     = this->client->scenegraph.findNode<Model>(flexVec[0].AsString().str());
	uint32_t submeshIndex            = flexVec[0].AsUInt32();

	if(model != nullptr)
		model->replaceMaterial(submeshIndex, surface->surfaceMatAlphaClip);
	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::close(flexbuffers::Reference data, bool returnValue) {
	surface->close();
	return std::vector<uint8_t>();
}

}
