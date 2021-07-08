#pragma once

#include "../spatialnode.hpp"

namespace StardustXRServer {

class DrawableNode : public SpatialNode {
public:
	DrawableNode() {
		STARDUSTXR_NODE_METHOD("setVisible", &DrawableNode::setVisible)
	}
	virtual ~DrawableNode() {}

	bool visible = true;
	std::vector<uint8_t> setVisible(uint sessionID, flexbuffers::Reference data, bool returnValue) {
		visible = data.AsBool();
	}
	virtual void draw() {}
};

} //namespace StardustXRServer
