#pragma once

#include "../spatialnode.hpp"

namespace StardustXRServer {

class DrawableNode : public SpatialNode {
public:
	DrawableNode() {}
	virtual ~DrawableNode() {}

	bool visible = true;
	virtual void draw() {}
};

} //namespace StardustXRServer
