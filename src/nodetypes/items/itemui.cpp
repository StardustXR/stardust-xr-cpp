#include "itemui.hpp"
#include "../../core/client.hpp"

namespace StardustXRServer {

ItemUI::ItemUI(Client *client, Item::TypeInfo *info, std::string callbackPath, std::string callbackMethod) : Node(client, false) {
	this->itemTypeInfo = info;

	this->callbackPath = callbackPath;
	this->callbackMethod = callbackMethod;

	info->UI = this;

	for(Item *item : info->items.list()) {
		handleItemCreate(item);
	}
}
ItemUI::~ItemUI() {
	itemTypeInfo->UI = nullptr;
}

void ItemUI::handleItemCreate(Item *item) {
	Alias *alias = item->makeAlias(client);
	addChild(item->name, alias);
	alias->addMethods({"triggerAccept", "release"});

	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&]() {
				fbb.UInt(0);
				fbb.String(item->name);
				item->serializeData(fbb);
			});
		}
	);
}

void ItemUI::handleItemCapture(Item *item) {
	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&]() {
				fbb.UInt(1);
				fbb.String(item->name);
			});
		}
	);
}

void ItemUI::handleItemRelease(Item *item) {
	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&]() {
				fbb.UInt(2);
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
				fbb.UInt(3);
				fbb.String(itemName);
			});
		}
	);
}

} // namespace StardustXRServer
