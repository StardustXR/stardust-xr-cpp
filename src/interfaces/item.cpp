#include "item.hpp"
#include "../core/client.hpp"
#include "../nodetypes/spatial/alias.hpp"
#include "../nodetypes/items/environment.hpp"
#include "../nodetypes/items/panel.hpp"
#include "../nodetypes/items/itemui.hpp"
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
	addChild("environment", new Node(client));
	addChild("panel", new Node(client));
}

std::vector<uint8_t> ItemInterface::createEnvironmentItem(flexbuffers::Reference data, bool returnValue) {
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
	internalParent->addChild(item->hashUUID(), item);

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

std::vector<uint8_t> ItemInterface::registerEnvironmentItemUI(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string callbackPath    = flexVec[0].AsString().str();
	std::string callbackMethod  = flexVec[1].AsString().str();

	if(EnvironmentItem::itemTypeInfo.UI == nullptr) {
		ItemUI *ui = new ItemUI(client, &EnvironmentItem::itemTypeInfo, callbackPath, callbackMethod);
		addChild("environment", ui);
	}

	return std::vector<uint8_t>();
}
std::vector<uint8_t> ItemInterface::registerPanelItemUI(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string callbackPath    = flexVec[0].AsString().str();
	std::string callbackMethod  = flexVec[1].AsString().str();

	if(PanelItem::itemTypeInfo.UI == nullptr) {
		ItemUI *ui = new ItemUI(client, &PanelItem::itemTypeInfo, callbackPath, callbackMethod);
		addChild("panel", ui);
	}

	return std::vector<uint8_t>();
}

std::vector<uint8_t> ItemInterface::createEnvironmentItemAcceptor(flexbuffers::Reference data, bool returnValue) {

	return std::vector<uint8_t>();
}
std::vector<uint8_t> ItemInterface::createPanelItemAcceptor(flexbuffers::Reference data, bool returnValue) {

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
