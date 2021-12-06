#include "itemui.hpp"
#include "../../core/client.hpp"
#include "../spatial/alias.hpp"

namespace StardustXRServer {

ItemUI::ItemUI(Client *client, Item::TypeInfo *info, std::string callbackPath, std::string callbackMethod) : Node(client, false) {
	this->itemTypeInfo = info;

	this->callbackPath = callbackPath;
	this->callbackMethod = callbackMethod;

	info->UI = this;

	std::lock_guard<std::mutex> lock(info->itemsMutex);
	for(Item *item : info->items) {
		handleItemCreate(item);
	}
}
ItemUI::~ItemUI() {
	itemTypeInfo->UI = nullptr;
}

void ItemUI::handleItemCreate(Item *item) {
	addChild(item->name, item->makeAlias(client));

	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&]() {
				fbb.Bool(true);
				fbb.String(item->name);
				item->serializeData(fbb);
			});
		}
	);
}

void ItemUI::handleItemDestroy(std::string itemName) {
	Node *alias = children[itemName].get();
	if(alias)
		alias->queueDestroy(false);

	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&]() {
				fbb.Bool(false);
				fbb.String(itemName);
			});
		}
	);
}

} // namespace StardustXRServer
