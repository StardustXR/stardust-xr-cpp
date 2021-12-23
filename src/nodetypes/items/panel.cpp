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

		"setPointerActive",
		"setPointerPosition",
		"setPointerButtonPressed",

		"resize",
		"close",
	}
};

PanelItem::PanelItem(Client *client, Surface *surface, pose_t pose) :
	Item(client, itemTypeInfo, pose),
	surface(surface) {
	STARDUSTXR_NODE_METHOD("applySurfaceMaterial", &PanelItem::applySurfaceMaterial)

	STARDUSTXR_NODE_METHOD("setPointerActive",        &PanelItem::setPointerActive)
	STARDUSTXR_NODE_METHOD("setPointerPosition",      &PanelItem::setPointerPosition)
	STARDUSTXR_NODE_METHOD("setPointerButtonPressed", &PanelItem::setPointerButtonPressed)

	STARDUSTXR_NODE_METHOD("resize", &PanelItem::resize)
	STARDUSTXR_NODE_METHOD("close", &PanelItem::close)
}
PanelItem::~PanelItem() {}

void PanelItem::update() {
	surface->frameEnd();
}

void PanelItem::serializeData(flexbuffers::Builder &fbb) {
	fbb.Vector([&]() {
		fbb.UInt(surface->width);
		fbb.UInt(surface->height);
	});
}

Alias *PanelItem::makeAlias(Client *client) {
	return new PanelAlias(client, this, itemTypeInfo.aliasedMethods);
}

std::vector<uint8_t> PanelItem::applySurfaceMaterial(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec      = data.AsVector();
	Model *model				     = this->client->scenegraph.findNode<Model>(flexVec[0].AsString().str());
	uint32_t submeshIndex            = flexVec[1].AsUInt32();

	if(model != nullptr)
		model->replaceMaterial(submeshIndex, surface->surfaceMatAlphaClip);
	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::setPointerActive(flexbuffers::Reference data, bool returnValue) {
	surface->setPointerActive(data.AsBool());

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setPointerPosition(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t x                  = flexVec[0].AsDouble();
	uint32_t y                  = flexVec[1].AsDouble();

	surface->setPointerPosition(x, y);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setPointerButtonPressed(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t button             = flexVec[0].AsUInt32();
	bool pressed                = flexVec[1].AsBool();

	surface->setPointerButtonPressed(button, pressed);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::resize(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t width              = flexVec[0].AsUInt32();
	uint32_t height             = flexVec[1].AsUInt32();

	surface->resize(width, height);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::close(flexbuffers::Reference data, bool returnValue) {
	surface->close();
	return std::vector<uint8_t>();
}

}
