#pragma once

#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/items/types/environment.hpp"

namespace StardustXRServer {

class ItemInterface : public Node {
public:
	ItemInterface(Client *client);

	static void updateItems();

	std::vector<uint8_t> createEnvironmentItem(flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> registerEnvironmentItemUI(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> registerPanelItemUI(flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> createItemAcceptor(flexbuffers::Reference data, bool returnValue, Item::TypeInfo *info);

	std::vector<uint8_t> createEnvironmentItemAcceptor(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createPanelItemAcceptor(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
