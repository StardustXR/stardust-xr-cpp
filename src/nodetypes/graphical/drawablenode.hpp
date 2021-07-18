#pragma once

#include "../spatial.hpp"
#include <vector>

namespace StardustXRServer {

class DrawableNode : public Spatial {
public:
	DrawableNode(Client *client) : Spatial(client) {
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
