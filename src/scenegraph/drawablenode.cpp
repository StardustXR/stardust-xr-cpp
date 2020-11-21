#include "drawablenode.hpp"

namespace StardustXRServer {

void DrawableNode::propagateDraw() {
	for(const auto child : children) {
		if(DrawableNode *nodeChild = dynamic_cast<DrawableNode *>(child.second)) { //Check if child node is the StardustXRServer node type
			nodeChild->draw();
			nodeChild->propagateDraw();
		}
	}
}

} // namespace StardustXRServer
