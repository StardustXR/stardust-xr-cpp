#include "item.hpp"
#include "itemui.hpp"
#include "../../core/client.hpp"
#include "../../core/clientmanager.hpp"
#include "../../globals.h"
#include "../spatial/alias.hpp"

namespace StardustXRServer {

Item::Item(Client *client, TypeInfo &itemTypeInfo, pose_t pose) :
	Spatial(client, nullptr, pose.position, pose.orientation, vec3_one, true, true, false, false),
	itemTypeInfo(&itemTypeInfo) {

	STARDUSTXR_NODE_METHOD("getData", &Item::getData)

	std::lock_guard<std::mutex> lock(itemTypeInfo.itemsMutex);
	itemTypeInfo.items.push_back(this);
}
Item::~Item() {
	if(itemTypeInfo->UI)
		itemTypeInfo->UI->handleItemDestroy(name);

	std::lock_guard<std::mutex> lock(itemTypeInfo->itemsMutex);
	itemTypeInfo->items.erase(std::remove(itemTypeInfo->items.begin(), itemTypeInfo->items.end(), this));
}

std::vector<uint8_t> Item::getData(flexbuffers::Reference data, bool returnValue) {
	return FLEX_SINGLE(
		serializeData(fbb);
	);
}

Alias *Item::makeAlias(Client *client) {
	return new SpatialAlias(client, this, itemTypeInfo->aliasedMethods);
}

}
