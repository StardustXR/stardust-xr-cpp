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

	void serializeData(flexbuffers::Builder &fbb);
	std::vector<uint8_t> applySurfaceMaterial(flexbuffers::Reference data, bool returnValue);

	static TypeInfo itemTypeInfo;
	const Surface *surface;
};

}
