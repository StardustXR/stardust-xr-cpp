#pragma once

#include "../core/noderef.hpp"
#include "../spatial/spatial.hpp"
#include "../fields/field.hpp"
#include "item.hpp"

namespace StardustXRServer {

class ItemAcceptor : public Spatial {
public:
	ItemAcceptor(Client *client, Spatial *spatialParent, sk::pose_t transform, Item::TypeInfo *typeInfo, Field *field, std::string callbackPath, std::string callbackMethod);
	~ItemAcceptor();

	NodeRef field;

	void captureItem(Item &item);
	void releaseItem(std::string itemName);

private:
	Item::TypeInfo *const typeInfo;

	std::string callbackPath;
	std::string callbackMethod;
};

}
