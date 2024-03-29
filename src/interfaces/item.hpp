#pragma once

#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/items/types/environment.hpp"

namespace StardustXRServer {

class ItemInterface : public Node {
public:
	ItemInterface(Client *client);

	static void updateItems();

	std::vector<uint8_t> createEnvironmentItem(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> registerEnvironmentItemUI(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> registerPanelItemUI(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> createEnvironmentItemAcceptor(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createPanelItemAcceptor(Client *callingClient, flexbuffers::Reference data, bool returnValue);

private:
	void addTypeNodes(Client *client, std::string name);
	std::vector<uint8_t> registerItemUI(flexbuffers::Reference data, Item::TypeInfo *info);
	std::vector<uint8_t> createItemAcceptor(flexbuffers::Reference data, Item::TypeInfo *info);
};

} // namespace StardustXRServer
