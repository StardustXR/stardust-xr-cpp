#pragma once

#include <string>
#include <vector>

#include "../spatial/spatial.hpp"

namespace StardustXRServer {

class ItemUI;

class Item : public Spatial {
public:
	struct TypeInfo {
		const std::string type;
		const std::vector<std::string> aliasedMethods;

		ItemUI *UI;

		std::vector<Item *> items;
		std::mutex itemsMutex;
	};

	Item(Client *client, TypeInfo &itemTypeInfo, sk::pose_t pose);
	virtual ~Item();

	std::vector<uint8_t> getData(flexbuffers::Reference data, bool returnValue);
	virtual void serializeData(flexbuffers::Builder &fbb) = 0;

	TypeInfo *itemTypeInfo;
};

}
