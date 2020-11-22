#include <stardustxr/server/messengermanager.hpp>
#include <stardustxr/server/stardust_scenegraph.hpp>

#include "tests/mesh.hpp"
using namespace StardustXRServer;

#include <stereokit.h>
using namespace sk;

StardustXR::ServerStardustScenegraph scenegraph;

std::function<void(Node *)> updateFunction = [](Node *node) {
	node->update();
};
std::function<void(Node *)> drawFunction = [](Node *node) {
	if(DrawableNode *drawNode = dynamic_cast<DrawableNode *>(node))
		drawNode->draw();
};

int main() {
	// log_set_filter(log_diagnostic);
	sk_init("Stardust XR", runtime_mixedreality);

	StardustXR::MessengerManager messengerManager(&scenegraph);

	TestMeshNode testNode;
	scenegraph.addNode("/mesh", &testNode);

	while (sk_step([]() {
		for(const auto &rawNode : scenegraph.root.children) {
			if(Node *node = dynamic_cast<Node *>(rawNode.second)) {
				node->propagate(updateFunction);
			}
		}
		for(const auto &rawDrawNode : scenegraph.root.children) {
			if(Node *drawNode = dynamic_cast<Node *>(rawDrawNode.second)) {
				drawNode->propagate(drawFunction);
			}
		}
	})) {}

	sk_shutdown();
	return 0;
}
