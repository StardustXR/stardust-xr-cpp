#pragma once

#include <string>
#include <vector>

#include "../spatial/spatial.hpp"

namespace StardustXRServer {

class ItemUI;
class ItemAcceptor;

class Item : public Spatial {
public:
	struct TypeInfo {
		const std::string type;
		const std::vector<std::string> aliasedMethods;

		ItemUI *UI;

		Registry<Item> items;
		Registry<ItemAcceptor> acceptors;
	};

	Item(Client *client, TypeInfo &itemTypeInfo, sk::pose_t transform);
	virtual ~Item();

	std::vector<uint8_t> getData(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	virtual void serializeData(flexbuffers::Builder &fbb) = 0;
	Alias *makeAlias(Client *client);

	std::vector<uint8_t> triggerAccept(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> release(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	static void updateItems(TypeInfo *info);
	virtual void update() {};

	bool acceptable = false;
	TypedNodeRef<ItemAcceptor> capturedAcceptor;
protected:
	TypeInfo *itemTypeInfo;
};

}
