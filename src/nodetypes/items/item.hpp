#pragma once

#include <string>
#include <vector>

#include "../core/noderef.hpp"
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

		std::vector<Item *> items;
		std::vector<ItemAcceptor *> acceptors;
		std::mutex itemsMutex;
	};

	Item(Client *client, TypeInfo &itemTypeInfo, sk::pose_t pose);
	virtual ~Item();

	std::vector<uint8_t> getData(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	virtual void serializeData(flexbuffers::Builder &fbb) = 0;
	Alias *makeAlias(Client *client);

	std::vector<uint8_t> triggerAccept(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> release(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	static void updateItems(TypeInfo *info);

	bool acceptable = false;
	NodeRef capturedAcceptor;
protected:
	TypeInfo *itemTypeInfo;
};

}
