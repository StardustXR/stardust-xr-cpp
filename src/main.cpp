// Globals includes
#include "globals.h"

// libstardustxr includes
#include <stardustxr/server/messengermanager.hpp>

// Stardust XR Server includes
#include "scenegraph/scenegraph.hpp"
#include "scenegraph/nodes/field.hpp"
#include "scenegraph/nodes/input.hpp"
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
FieldInterface field;
InputInterface input;
LifeCycleInterface lifeCycle;
ModelInterface model;

// Define lambda functions for update and draw functions to be propagated
PropagateFunction updateFunction = [](std::string, Node *node) {
	if(node)
		node->update();
	return node;
};
PropagateFunction drawFunction = [](std::string, Node *node) {
	if(DrawableNode *drawNode = dynamic_cast<DrawableNode *>(node))
		drawNode->draw();
	return true;
};

int main(int argc, char *argv[]) {
	// log_set_filter(log_diagnostic);
	sk_settings_t settings;

	if(argc > 1 && (strcmp("-F", argv[1]) || strcmp("--flatscreen", argv[1]))) {
		settings.app_name = "Stardust XR (Flatscreen)";
		settings.display_preference = display_mode_flatscreen;
	} else {
		settings.app_name = "Stardust XR";
		settings.display_preference = display_mode_mixedreality;
	}
	sk_init(settings);

	// Add the nodes to the scenegraph
	scenegraph.addNode("/field", &field);
	scenegraph.addNode("/input", &input);
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
