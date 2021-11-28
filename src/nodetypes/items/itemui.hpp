#pragma once

#include "../core/node.hpp"
#include "item.hpp"

namespace StardustXRServer {

class ItemUI : public Node {
public:
	ItemUI(Client *client, Item::TypeInfo *info, std::string callbackPath, std::string callbackMethod);
	~ItemUI();

	void handleItemCreate(Item *item);
	void handleItemDestroy(std::string itemName);

private:
	std::string callbackPath = "";
	std::string callbackMethod = "";

	Item::TypeInfo *itemTypeInfo;
};

} // namespace StardustXRServer
