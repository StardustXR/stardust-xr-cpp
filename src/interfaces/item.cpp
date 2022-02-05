#include "item.hpp"
#include "../core/client.hpp"
#include "../nodetypes/fields/field.hpp"
#include "../nodetypes/spatial/alias.hpp"
#include "../nodetypes/items/types/environment.hpp"
#include "../nodetypes/items/types/panel.hpp"
#include "../nodetypes/items/itemui.hpp"
#include "../nodetypes/items/acceptor.hpp"
#include "../globals.h"


#include <list>
#include <limits>

using namespace sk;
using namespace std;

namespace StardustXRServer {

ItemInterface::ItemInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("createEnvironmentItem", &ItemInterface::createEnvironmentItem)
	STARDUSTXR_NODE_METHOD("registerEnvironmentItemUI", &ItemInterface::registerEnvironmentItemUI)
	STARDUSTXR_NODE_METHOD("registerPanelItemUI", &ItemInterface::registerPanelItemUI)
	STARDUSTXR_NODE_METHOD("createEnvironmentItemAcceptor", &ItemInterface::createEnvironmentItemAcceptor)
	STARDUSTXR_NODE_METHOD("createPanelItemAcceptor", &ItemInterface::createPanelItemAcceptor)
	addChild("acceptor", new Node(client));
	addChild("environment", new Node(client));
	addChild("panel", new Node(client));
}

void ItemInterface::updateItems() {
	Item::updateItems(&EnvironmentItem::itemTypeInfo);
	Item::updateItems(&PanelItem::itemTypeInfo);
}

std::vector<uint8_t> ItemInterface::createEnvironmentItem(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec      = data.AsVector();
	std::string name                 = flexVec[0].AsString().str();
	Spatial *space                   = this->client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	std::string path                 = flexVec[2].AsString().str();
	flexbuffers::TypedVector flexPos = flexVec[3].AsTypedVector();
	flexbuffers::TypedVector flexRot = flexVec[4].AsTypedVector();

	sk::vec3 pos = {
		flexPos[0].AsFloat(),
		flexPos[1].AsFloat(),
		flexPos[2].AsFloat()
	};
	sk::quat rot = {
		flexRot[0].AsFloat(),
		flexRot[1].AsFloat(),
		flexRot[2].AsFloat(),
		flexRot[3].AsFloat()
	};
	sk::pose_t pose = {pos, rot};

	EnvironmentItem *item = new EnvironmentItem(&serverInternalClient, path, space ? matrix_transform_pose(space->worldTransform(), pose) : pose);

	Node *internalParent = serverInternalClient.scenegraph.findNode("/item/environment/");
	internalParent->addChild(std::to_string(item->id), item);

	if(EnvironmentItem::itemTypeInfo.UI)
		EnvironmentItem::itemTypeInfo.UI->handleItemCreate(item);

	SpatialAlias *itemAlias = new SpatialAlias(client, item, {
		"move",
		"rotate",
		"setOrigin",
		"setOrientation",
		"setPose"
	});
	children["environment"]->addChild(name, itemAlias);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> ItemInterface::registerEnvironmentItemUI(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string callbackPath    = flexVec[0].AsString().str();
	std::string callbackMethod  = flexVec[1].AsString().str();

	if(EnvironmentItem::itemTypeInfo.UI == nullptr) {
		ItemUI *ui = new ItemUI(client, &EnvironmentItem::itemTypeInfo, callbackPath, callbackMethod);
		addChild("environment", ui);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> ItemInterface::registerPanelItemUI(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string callbackPath    = flexVec[0].AsString().str();
	std::string callbackMethod  = flexVec[1].AsString().str();

	if(PanelItem::itemTypeInfo.UI == nullptr) {
		ItemUI *ui = new ItemUI(client, &PanelItem::itemTypeInfo, callbackPath, callbackMethod);
		addChild("panel", ui);
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> ItemInterface::createItemAcceptor(Client *callingClient, flexbuffers::Reference data, bool returnValue, Item::TypeInfo *info) {
	flexbuffers::Vector flexVec      = data.AsVector();
	std::string name                 = flexVec[0].AsString().str();
	Spatial *space                   = this->client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	Field *field                     = this->client->scenegraph.findNode<Field>(flexVec[2].AsString().str());
	flexbuffers::TypedVector flexPos = flexVec[3].AsTypedVector();
	flexbuffers::TypedVector flexRot = flexVec[4].AsTypedVector();
	std::string callbackPath         = flexVec[5].AsString().str();
	std::string callbackMethod       = flexVec[6].AsString().str();

	sk::vec3 pos = {
		flexPos[0].AsFloat(),
		flexPos[1].AsFloat(),
		flexPos[2].AsFloat()
	};
	sk::quat rot = {
		flexRot[0].AsFloat(),
		flexRot[1].AsFloat(),
		flexRot[2].AsFloat(),
		flexRot[3].AsFloat()
	};

	ItemAcceptor *acceptor = new ItemAcceptor(client, space, pos, rot, info, field, callbackPath, callbackMethod);
	children["acceptor"]->addChild(name, acceptor);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> ItemInterface::createEnvironmentItemAcceptor(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return createItemAcceptor(callingClient, data, returnValue, &EnvironmentItem::itemTypeInfo);
}
std::vector<uint8_t> ItemInterface::createPanelItemAcceptor(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return createItemAcceptor(callingClient, data, returnValue, &PanelItem::itemTypeInfo);
}

} // namespace StardustXRServer
