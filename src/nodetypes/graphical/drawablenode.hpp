#pragma once

#include "../spatial.hpp"
#include <vector>

namespace StardustXRServer {

class DrawableNode : public Spatial {
public:
	DrawableNode(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, sk::vec3 scale, bool translatable, bool rotatable, bool scalable) : Spatial(client, spatialParent, position, rotation, scale, translatable, rotatable, scalable) {
		STARDUSTXR_NODE_METHOD("setVisible", &DrawableNode::setVisible)
	}
	virtual ~DrawableNode() {}

	bool visible = true;
	std::vector<uint8_t> setVisible(flexbuffers::Reference data, bool returnValue) {
		visible = data.AsBool();
		return std::vector<uint8_t>();
	}
	virtual void draw() {}
};

} //namespace StardustXRServer
