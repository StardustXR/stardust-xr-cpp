#include <stardustxr/server/messengermanager.hpp>

#include "scenegraph/scenegraph.hpp"
#include "scenegraph/nodes/model.hpp"
using namespace StardustXRServer;

#include <stereokit.h>
using namespace sk;

Scenegraph scenegraph;

std::function<void(Node *)> updateFunction = [](Node *node) {
	node->update();
};
std::function<void(Node *)> drawFunction = [](Node *node) {
	if(DrawableNode *drawNode = dynamic_cast<DrawableNode *>(node))
		drawNode->draw();
};


int main(int argc, char *argv[]) {
	// log_set_filter(log_diagnostic);
	if(argc > 1 && (strcmp("-F", argv[1]) || strcmp("--flatscreen", argv[1]))) {
		sk_init("Stardust XR (Flatscreen)", runtime_flatscreen);
	} else {
		sk_init("Stardust XR", runtime_mixedreality);
	}

	StardustXR::MessengerManager messengerManager(&scenegraph);


	ModelInterface model;
	scenegraph.addNode("/model", &model);

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
