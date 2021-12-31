#include "panel.hpp"
#include "../../../core/client.hpp"
#include "../../drawable/model.hpp"
#include "../../../integrations/wayland/surface.hpp"
#include "../alias.hpp"

namespace StardustXRServer {

Item::TypeInfo PanelItem::itemTypeInfo = {
	"panel",
	{
		"applySurfaceMaterial",
		"getData",

		"setPointerActive",
		"setPointerPosition",
		"setPointerButtonPressed",
		"scrollPointerAxis",

		"touchDown",
		"touchMove",
		"touchUp",

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
	STARDUSTXR_NODE_METHOD("scrollPointerAxis",       &PanelItem::scrollPointerAxis)

	STARDUSTXR_NODE_METHOD("touchDown", &PanelItem::touchDown)
	STARDUSTXR_NODE_METHOD("touchMove", &PanelItem::touchMove)
	STARDUSTXR_NODE_METHOD("touchUp",   &PanelItem::touchUp)

	STARDUSTXR_NODE_METHOD("resize", &PanelItem::resize)
	STARDUSTXR_NODE_METHOD("close", &PanelItem::close)
}
PanelItem::~PanelItem() {}

void PanelItem::update() {
	surface->frameEnd();
}

void PanelItem::serializeData(flexbuffers::Builder &fbb) {
	fbb.Vector([&]() {
		fbb.UInt(getEnabled() ? surface->width  : 0);
		fbb.UInt(getEnabled() ? surface->height : 0);
	});
}

Alias *PanelItem::makeAlias(Client *client) {
	return new PanelAlias(client, this, itemTypeInfo.aliasedMethods);
}

std::vector<uint8_t> PanelItem::applySurfaceMaterial(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec      = data.AsVector();
	Model *model				     = this->client->scenegraph.findNode<Model>(flexVec[0].AsString().str());
	uint32_t submeshIndex            = flexVec[1].AsUInt32();

	if(model != nullptr)
		model->replaceMaterial(submeshIndex, surface->surfaceMatAlphaClip);
	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::setPointerActive(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	surface->setPointerActive(data.AsBool());

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setPointerPosition(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t x                  = flexVec[0].AsDouble();
	uint32_t y                  = flexVec[1].AsDouble();

	surface->setPointerPosition(x, y);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setPointerButtonPressed(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t button             = flexVec[0].AsUInt32();
	bool pressed                = flexVec[1].AsBool();

	surface->setPointerButtonPressed(button, pressed);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::scrollPointerAxis(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t source             = flexVec[0].AsUInt32();
	  double x                  = flexVec[1].AsDouble();
	  double y                  = flexVec[2].AsDouble();
	 int32_t dx                 = flexVec[3].AsInt32();
	 int32_t dy                 = flexVec[4].AsInt32();

	surface->scrollPointerAxis(source, x, y, dx, dy);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::touchDown(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t id                 = flexVec[0].AsUInt32();
	  double x                  = flexVec[1].AsDouble();
	  double y                  = flexVec[2].AsDouble();

	surface->touchDown(id, x, y);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::touchMove(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t id                 = flexVec[0].AsUInt32();
	  double x                  = flexVec[1].AsDouble();
	  double y                  = flexVec[2].AsDouble();

	surface->touchMove(id, x, y);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::touchUp(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	uint32_t id = data.AsUInt32();

	surface->touchUp(id);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::resize(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t width              = flexVec[0].AsUInt32();
	uint32_t height             = flexVec[1].AsUInt32();

	surface->resize(width, height);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::close(flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	surface->close();
	return std::vector<uint8_t>();
}

}
