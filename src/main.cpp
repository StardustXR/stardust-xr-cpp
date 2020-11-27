// Globals includes
#include "globals.h"

// libstardustxr includes
#include <stardustxr/server/messengermanager.hpp>

// Stardust XR Server includes
#include "scenegraph/scenegraph.hpp"
#include "scenegraph/nodes/lifecycle.hpp"
#include "scenegraph/nodes/model.hpp"
using namespace StardustXRServer;

// StereoKit includes
#include <stereokit.h>
using namespace sk;

// Initialize scnenegraph and messenger manager
Scenegraph scenegraph;
StardustXR::MessengerManager messengerManager(&scenegraph);

// Initialize scenegraph object nodes
LifeCycleInterface lifeCycle;
ModelInterface model;

// Define lambda functions for update and draw functions to be propagated
PropagateFunction updateFunction = [](std::string name, Node *node) {
	if(node)
		node->update();
	return node;
};
PropagateFunction drawFunction = [](std::string name, Node *node) {
	if(DrawableNode *drawNode = dynamic_cast<DrawableNode *>(node))
		drawNode->draw();
	return true;
};

int main(int argc, char *argv[]) {
	// log_set_filter(log_diagnostic);
	if(argc > 1 && (strcmp("-F", argv[1]) || strcmp("--flatscreen", argv[1]))) {
		sk_init("Stardust XR (Flatscreen)", runtime_flatscreen);
	} else {
		sk_init("Stardust XR", runtime_mixedreality);
	}

	// Add the nodes to the scenegraph
	scenegraph.addNode("/lifecycle", &lifeCycle);
	scenegraph.addNode("/model", &model);

	// Every stereokit step
	while (sk_step([]() {
		// Send logicStep signals to clients
		lifeCycle.sendLogicStepSignals();

		//Propagate the update and draw methods on scenegraph nodes
		scenegraph.root.propagate("", updateFunction);
		scenegraph.root.propagate("", drawFunction);
	})) {}

	sk_shutdown();
	return 0;
}
