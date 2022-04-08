#include "item.hpp"
#include "../core/client.hpp"
#include "../nodetypes/core/alias.hpp"
#include "../nodetypes/fields/field.hpp"
#include "../nodetypes/items/types/environment.hpp"
#include "../nodetypes/items/types/panel.hpp"
#include "../nodetypes/items/itemui.hpp"
#include "../nodetypes/items/acceptor.hpp"
#include "../util/flex.hpp"
#include "../globals.hpp"


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

	addTypeNodes(client, "environment");
	addTypeNodes(client, "panel");
}

void ItemInterface::addTypeNodes(Client *client, std::string name) {
	Node *type = new Node(client);
	addChild(name, type);

	type->addChild("acceptor", new Node(client));
	type->addChild("item", new Node(client));
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
	pose_t transform                 = FlexToSKPose(flexVec[3].AsTypedVector(), flexVec[4].AsTypedVector());

	EnvironmentItem *item = new EnvironmentItem(serverInternalClient, path, space ? matrix_transform_pose(space->worldTransform(), transform) : transform);

	Node *internalParent = serverInternalClient->scenegraph.findNode("/item/environment/");
	internalParent->addChild(std::to_string(item->id), item);

	if(EnvironmentItem::itemTypeInfo.UI)
		EnvironmentItem::itemTypeInfo.UI->handleItemCreate(item);

	children["environment"]->children["item"]->addChild(name, item->makeAlias(client));

	return std::vector<uint8_t>();
}

std::vector<uint8_t> ItemInterface::registerItemUI(flexbuffers::Reference data, Item::TypeInfo *info) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string callbackPath    = flexVec[0].AsString().str();
	std::string callbackMethod  = flexVec[1].AsString().str();

	if(!info->UI) {
		ItemUI *ui = new ItemUI(client, info, callbackPath, callbackMethod);
		children[info->type]->addChild("ui", ui);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> ItemInterface::registerEnvironmentItemUI(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return registerItemUI(data, &EnvironmentItem::itemTypeInfo);
}
std::vector<uint8_t> ItemInterface::registerPanelItemUI(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return registerItemUI(data, &PanelItem::itemTypeInfo);
}

std::vector<uint8_t> ItemInterface::createItemAcceptor(flexbuffers::Reference data, Item::TypeInfo *info) {
	flexbuffers::Vector flexVec      = data.AsVector();
	std::string name                 = flexVec[0].AsString().str();
	Spatial *space                   = client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	Field *field                     = client->scenegraph.findNode<Field>(flexVec[2].AsString().str());
	pose_t transform                 = FlexToSKPose(flexVec[3].AsTypedVector(), flexVec[4].AsTypedVector());
	std::string callbackPath         = flexVec[5].AsString().str();
	std::string callbackMethod       = flexVec[6].AsString().str();

	ItemAcceptor *acceptor = new ItemAcceptor(client, space, transform, info, field, callbackPath, callbackMethod);
	children[info->type]->children["acceptor"]->addChild(name, acceptor);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> ItemInterface::createEnvironmentItemAcceptor(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return createItemAcceptor(data, &EnvironmentItem::itemTypeInfo);
}
std::vector<uint8_t> ItemInterface::createPanelItemAcceptor(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return createItemAcceptor(data, &PanelItem::itemTypeInfo);
}

}
