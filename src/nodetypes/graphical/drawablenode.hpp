#ifndef STARDUSTXR_DRAWABLE_NODE_H
#define STARDUSTXR_DRAWABLE_NODE_H

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

#endif //STARDUSTXR_DRAWABLE_NODE_H
