#ifndef STARDUSTXR_DRAWABLE_NODE_H
#define STARDUSTXR_DRAWABLE_NODE_H

#include "node.hpp"

namespace StardustXRServer {

class DrawableNode : public Node {
public:
	DrawableNode() {}
	virtual ~DrawableNode() {}

	virtual void draw() = 0;
};

} //namespace StardustXRServer

#endif //STARDUSTXR_DRAWABLE_NODE_H
