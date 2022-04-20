#include "panel.hpp"
#include "../../../core/client.hpp"
#include "../../drawable/model.hpp"
#include "../../../integrations/wayland/surface.hpp"
#include "../../core/alias.hpp"

using namespace sk;

namespace StardustXRServer {

Item::TypeInfo PanelItem::itemTypeInfo = {
	"panel",
	{
		"applySurfaceMaterial",

		"setPointerActive",
		"setPointerPosition",
		"setPointerButtonPressed",
		"scrollPointerAxis",

		"touchDown",
		"touchMove",
		"touchUp",

		"setKeyboardActive",
		"setKeymap",
		"setKeyState",
		"setKeyModStates",
		"setKeyRepeat",

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

	STARDUSTXR_NODE_METHOD("setKeyboardActive", &PanelItem::setKeyboardActive)
	STARDUSTXR_NODE_METHOD("setKeymap",         &PanelItem::setKeymap)
	STARDUSTXR_NODE_METHOD("setKeyState",       &PanelItem::setKeyState)
	STARDUSTXR_NODE_METHOD("setKeyModStates",   &PanelItem::setKeyModStates)
	STARDUSTXR_NODE_METHOD("setKeyRepeat",      &PanelItem::setKeyRepeat)

	STARDUSTXR_NODE_METHOD("resize", &PanelItem::resize)
	STARDUSTXR_NODE_METHOD("close", &PanelItem::close)
}
PanelItem::~PanelItem() {}

void PanelItem::serializeData(flexbuffers::Builder &fbb) {
	fbb.Vector([&]() {
		fbb.UInt(getEnabled() ? surface->width  : 0);
		fbb.UInt(getEnabled() ? surface->height : 0);
	});
}

std::vector<uint8_t> PanelItem::applySurfaceMaterial(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec      = data.AsVector();
	Model *model				     = callingClient->scenegraph.findNode<Model>(flexVec[0].AsString().str());
	uint32_t submeshIndex            = flexVec[1].AsUInt32();

	if(model != nullptr)
		model->replaceMaterial(submeshIndex, surface->surfaceMatAlphaClip);
	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::setPointerActive(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	surface->setPointerActive(data.AsBool());

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setPointerPosition(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t x                  = flexVec[0].AsDouble();
	uint32_t y                  = flexVec[1].AsDouble();

	surface->setPointerPosition(x, y);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setPointerButtonPressed(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t button             = flexVec[0].AsUInt32();
	bool pressed                = flexVec[1].AsBool();

	surface->setPointerButtonPressed(button, pressed);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::scrollPointerAxis(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
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

std::vector<uint8_t> PanelItem::touchDown(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t id                 = flexVec[0].AsUInt32();
	  double x                  = flexVec[1].AsDouble();
	  double y                  = flexVec[2].AsDouble();

	surface->touchDown(id, x, y);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::touchMove(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t id                 = flexVec[0].AsUInt32();
	  double x                  = flexVec[1].AsDouble();
	  double y                  = flexVec[2].AsDouble();

	surface->touchMove(id, x, y);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::touchUp(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	uint32_t id = data.AsUInt32();

	surface->touchUp(id);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::setKeyboardActive(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	surface->setKeyboardActive(data.AsBool());

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setKeymap(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	surface->setKeymap(data.AsString().str());

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setKeyState(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	surface->setKeyState(flexVec[0].AsUInt32(), flexVec[1].AsUInt32());

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setKeyModStates(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	surface->setKeyModStates(
		flexVec[0].AsUInt32(),
		flexVec[1].AsUInt32(),
		flexVec[2].AsUInt32(),
		flexVec[3].AsUInt32()
	);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> PanelItem::setKeyRepeat(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	surface->setKeyRepeat(flexVec[0].AsInt32(), flexVec[1].AsInt32());

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::resize(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	flexbuffers::Vector flexVec = data.AsVector();
	uint32_t width              = flexVec[0].AsUInt32();
	uint32_t height             = flexVec[1].AsUInt32();

	surface->resize(width, height);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> PanelItem::close(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(!getEnabled())
		return std::vector<uint8_t>();

	surface->close();
	return std::vector<uint8_t>();
}

}
