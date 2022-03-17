#include "acceptor.hpp"
#include "itemui.hpp"
#include "../core/alias.hpp"
#include "../../core/client.hpp"
#include <mutex>

namespace StardustXRServer {

ItemAcceptor::ItemAcceptor(Client *client, Spatial *spatialParent, vec3 position, quat rotation, Item::TypeInfo *typeInfo, Field *field, std::string callbackPath, std::string callbackMethod) :
	Spatial(client, spatialParent, position, rotation, vec3_one, true, true, false, false),
	field(field),
	typeInfo(typeInfo),
	callbackPath(callbackPath),
	callbackMethod(callbackMethod) {

	typeInfo->acceptors.add(this);
}
ItemAcceptor::~ItemAcceptor() {
	for(Item *item : typeInfo->items.list()) {
		if(item->capturedAcceptor.ptr() == this) {
			typeInfo->UI->handleItemRelease(item);
			releaseItem(item->name);
		}
	}
	typeInfo->acceptors.remove(this);
}

void ItemAcceptor::captureItem(Item &item) {
	Alias *alias = item.makeAlias(client);
	alias->addMethods({"release"});
	addChild(alias->original->name, alias);

	item.capturedAcceptor = NodeRef(this);

	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&]() {
				fbb.Bool(true);
				fbb.String(alias->name);
				item.serializeData(fbb);
			});
		}
	);
}
void ItemAcceptor::releaseItem(std::string itemName) {
	Alias *alias = dynamic_cast<Alias *>(children[itemName].get());
	if(alias) {
		Item *item = alias->original.ptr<Item>();
		if(item) {
			item->capturedAcceptor = NodeRef();
		}
	}
	children.erase(itemName);

	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&]() {
				fbb.Bool(false);
				fbb.String(alias->name);
			});
		}
	);
}

}
