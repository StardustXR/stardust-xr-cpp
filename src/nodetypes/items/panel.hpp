#pragma once

#include <string>
#include <vector>

#include "item.hpp"

class Surface;

namespace StardustXRServer {

class PanelItem : public Item {
public:
	PanelItem(Client *client, Surface *surface, sk::pose_t pose);
	~PanelItem();

	void update();

	void serializeData(flexbuffers::Builder &fbb);
	Alias *makeAlias(Client *client);
	std::vector<uint8_t> applySurfaceMaterial(flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> setPointerActive(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setPointerPosition(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setPointerButtonPressed(flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> resize(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> close(flexbuffers::Reference data, bool returnValue);

	static TypeInfo itemTypeInfo;
	Surface *surface;
};

}
