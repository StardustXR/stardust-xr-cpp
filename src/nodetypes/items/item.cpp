#include "item.hpp"
#include "acceptor.hpp"
#include "itemui.hpp"
#include "../../core/client.hpp"
#include "../../core/clientmanager.hpp"
#include "../../globals.h"
#include "../core/alias.hpp"

namespace StardustXRServer {

Item::Item(Client *client, TypeInfo &itemTypeInfo, pose_t pose) :
	Spatial(client, nullptr, pose.position, pose.orientation, vec3_one, true, true, false, false),
	itemTypeInfo(&itemTypeInfo) {

	STARDUSTXR_NODE_METHOD("getData", &Item::getData)
	STARDUSTXR_NODE_METHOD("triggerAccept", &Item::triggerAccept)
	STARDUSTXR_NODE_METHOD("release", &Item::release)

	itemTypeInfo.items.add(this);
}
Item::~Item() {
	if(itemTypeInfo->UI)
		itemTypeInfo->UI->handleItemDestroy(name);

	itemTypeInfo->items.remove(this);
}

std::vector<uint8_t> Item::getData(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	return FLEX_SINGLE(
		serializeData(fbb);
	);
}

Alias *Item::makeAlias(Client *client) {
	Alias *alias = new Alias(client, this, itemTypeInfo->aliasedMethods);

	alias->addMethods({
		"getTransform",
		"setTransform",
		"setSpatialParent",
		"setSpatialParentInPlace",
		"setZoneable",
	});

	return alias;
}

std::vector<uint8_t> Item::triggerAccept(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	acceptable = true;
	return std::vector<uint8_t>();
}

std::vector<uint8_t> Item::release(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	if(capturedAcceptor) {
		itemTypeInfo->UI->handleItemRelease(this);
		capturedAcceptor.ptr<ItemAcceptor>()->releaseItem(name);
	}
	return std::vector<uint8_t>();
}

void Item::updateItems(TypeInfo *info) {
	for(Item *item : info->items.list()) {
		if(item->capturedAcceptor || !item->acceptable)
			continue;
		item->acceptable = false;
		ItemAcceptor *closestAcceptor = nullptr;
		float closestAcceptorDistance = 0;
		for(ItemAcceptor *acceptor : info->acceptors.list()) {
			if(!acceptor->field)
				continue;
			Field *acceptorField = acceptor->field.ptr<Field>();
			float acceptorDistance = acceptorField->distance(item, vec3_zero);
			if(acceptorDistance < closestAcceptorDistance) {
				closestAcceptorDistance = acceptorDistance;
				closestAcceptor = acceptor;
			}
		}
		if(closestAcceptor) {
			item->itemTypeInfo->UI->handleItemCapture(item);
			closestAcceptor->captureItem(*item);
		}
		item->update();
	}
}

}
