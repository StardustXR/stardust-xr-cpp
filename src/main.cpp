// Globals includes
#include "globals.h"

// libstardustxr includes
#include <stardustxr/server/messengermanager.hpp>

// Stardust XR Server includes
#include "scenegraph/scenegraph.hpp"
#include "scenegraph/nodes/environment.hpp"
#include "scenegraph/nodes/field.hpp"
#include "scenegraph/nodes/input.hpp"
#include "scenegraph/nodes/lifecycle.hpp"
#include "scenegraph/nodes/model.hpp"
#include "scenegraph/nodes/spatial.hpp"
#include "tests/flatscreenpointer.hpp"
#include "tests/skhand.hpp"
using namespace StardustXRServer;

// StereoKit includes
#include <stereokit.h>
using namespace sk;

// Global variables
bool flatscreen = false;

// Initialize scnenegraph and messenger manager
Scenegraph scenegraph;
StardustXR::MessengerManager messengerManager(&scenegraph);

// Initialize scenegraph object nodes
EnvironmentInterface environment;
FieldInterface field;
InputInterface input;
LifeCycleInterface lifeCycle;
ModelInterface model;
SpatialFactory spatial;

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
	sk_settings_t settings = {};
	settings.assets_folder = "";
	settings.log_filter = log_diagnostic;

	if(argc > 1 && (strcmp("-F", argv[1]) || strcmp("--flatscreen", argv[1]))) {
		settings.app_name = "Stardust XR (Flatscreen)";
		settings.display_preference = display_mode_flatscreen;
		flatscreen = true;
	} else {
		settings.app_name = "Stardust XR";
		settings.display_preference = display_mode_mixedreality;
		flatscreen = false;
	}
	if(!sk_init(settings))
		perror("Stereokit initialization failed!");

	// Add the nodes to the scenegraph
	scenegraph.addNode("/environment", &environment);
	scenegraph.addNode("/field", &field);
	scenegraph.addNode("/input", &input);
	scenegraph.addNode("/lifecycle", &lifeCycle);
	scenegraph.addNode("/model", &model);
	scenegraph.addNode("/spatial", &spatial);
	if(flatscreen) { // Add the flatscreen pointer if we're in flatscreen mode
		input_hand_visible(handed_right, false);
		FlatscreenPointer *flatscreenPointer = new FlatscreenPointer();
		scenegraph.addNode("/test/flatscreenpointer", static_cast<SpatialNode *>(flatscreenPointer));
		input.inputMethods.pushBack(flatscreenPointer);
		input.inputMethods.done();
	} else { // Add the StereoKit hand representation if we're not in flatscreen
		SKHandInput *stereokitHands[2];
//		stereokitHands[0] = new SKHandInput(handed_left);
		stereokitHands[1] = new SKHandInput(handed_right);
//		scenegraph.addNode("/test/skhandleft", static_cast<SpatialNode *>(stereokitHands[0]));
		scenegraph.addNode("/test/skhandright", static_cast<SpatialNode *>(stereokitHands[1]));
//		input.inputMethods.pushBack(stereokitHands[0]);
		input.inputMethods.pushBack(stereokitHands[1]);
		input.inputMethods.done();
	}

	// Every stereokit step
	while (sk_step([]() {
		// Send logicStep signals to clients
		lifeCycle.sendLogicStepSignals();

		//Propagate the update and draw methods on scenegraph nodes
		scenegraph.root.propagate("", updateFunction);
		scenegraph.root.propagate("", drawFunction);

		// Process all the input and send it to the clients
		input.processInput();
	})) {}

	sk_shutdown();
	return 0;
}
