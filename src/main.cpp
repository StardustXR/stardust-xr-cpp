// Globals includes
#include "globals.h"

// Stardust XR Server includes
#include "core/clientmanager.hpp"
#include "core/scenegraphpropagation.hpp"
#include "interfaces/environment.hpp"
#include "interfaces/lifecycle.hpp"
#include "interfaces/input.hpp"
#include "nodetypes/node.hpp"
#include "nodetypes/graphical/drawablenode.hpp"
#include "tests/flatscreenpointer.hpp"
#include "tests/skhand.hpp"
using namespace StardustXRServer;

// StereoKit includes
#include <stereokit.h>
using namespace sk;

// Third party local includes
#include "CLI11.hpp"

// Global variables
int CLIArgs::parse(int argc, const char* const argv[]) {
	CLI::App app("Stardust XR");
	app.add_flag("-F,--flatscreen", flatscreen, "Run Stardust in flatscreen mode");
	app.add_flag("--field-debug", fieldDebug, "Draw translucent meshes around fields");
	try {
		(app).parse((argc), (argv));
	} catch(const CLI::ParseError &e) {
		return (app).exit(e);
	}
	return -1;
}
CLIArgs args;
extern void debugSetup();

// Initialize scenegraph and client manager
ClientManager clientManager;
Client serverInternalClient(0, 0, &clientManager);

int main(int argc, char *argv[]) {
	int parse_result = args.parse(argc, argv);
	if (parse_result != -1) return parse_result;

	sk_settings_t settings = {};
	settings.assets_folder = "";
	settings.log_filter = log_diagnostic;
	settings.app_name = args.flatscreen ? "Stardust XR (Flatscreen)" : "Stardust XR";
	settings.display_preference = args.flatscreen ? display_mode_flatscreen : display_mode_mixedreality;

	if(!sk_init(settings))
		perror("Stereokit initialization failed!");
	gradient_t skycolor = gradient_create();
	gradient_add(skycolor, {0, 0, 0, 1}, 0.0f);
	gradient_add(skycolor, {0, 0, 0, 1}, 1.0f);
	tex_t skytex = tex_gen_cubemap(skycolor, vec3_up, 2);
	render_set_skytex(skytex);

	// Set up debugging
	if(args.fieldDebug)
		debugSetup();

	if(args.flatscreen) { // Add the flatscreen pointer if we're in flatscreen mode
		input_hand_visible(handed_right, false);
		FlatscreenPointer *flatscreenPointer = new FlatscreenPointer(&serverInternalClient);
		serverInternalClient.scenegraph.addNode("/test/flatscreenpointer", static_cast<Spatial *>(flatscreenPointer));
		InputInterface::addInputMethod(flatscreenPointer);
	} else { // Add the StereoKit hand representation if we're not in flatscreen
		SKHandInput *stereokitHands[2];
//		stereokitHands[0] = new SKHandInput(handed_left);
		stereokitHands[1] = new SKHandInput(&serverInternalClient, handed_right);
//		scenegraph.addNode("/test/skhandleft", static_cast<SpatialNode *>(stereokitHands[0]));
		serverInternalClient.scenegraph.addNode("/test/skhandright", static_cast<Spatial *>(stereokitHands[1]));
//		input.inputMethods.pushBack(stereokitHands[0]);
		InputInterface::addInputMethod(stereokitHands[1]);
	}

	// Every stereokit step
	while (sk_step([]() {
		// Handle disconnected clients before anything else to ensure scenegraph is clean
		clientManager.handleDisconnectedClients();
		clientManager.handleNewlyConnectedClients();

		// Update environment settings
		EnvironmentInterface::updateEnvironment();

		// Send logicStep signals to clients
		LifeCycleInterface::sendLogicStepSignals();

		//Propagate the update and draw methods on scenegraph nodes
		serverInternalClient.scenegraph.root.propagate("", ScenegraphUpdateFunction);
		clientManager.callClientsUpdate();
		serverInternalClient.scenegraph.root.propagate("", ScenegraphDrawFunction);
		clientManager.callClientsDraw();

		// Propagate the debug draw methods if the appropriate attribute is set
		if(args.fieldDebug) {
			serverInternalClient.scenegraph.root.propagate("", ScenegraphDebugFunction);
			clientManager.callClientsDebug();
		}

		// Process all the input and send it to the clients
		InputInterface::processInput();
	})) {}

	sk_shutdown();
	return 0;
}
