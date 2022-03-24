// Globals includes
#include "globals.h"

// Home directory getters
#include <pwd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdint>
#include <string>

std::string home;

// Stardust XR Server includes
#include "core/clientmanager.hpp"
#include "core/scenegraphpropagation.hpp"
#include "interfaces/drawable.hpp"
#include "interfaces/input.hpp"
#include "interfaces/item.hpp"
#include "interfaces/root.hpp"
#include "interfaces/spatial.hpp"
#include "nodetypes/core/node.hpp"
#include "nodetypes/drawable/drawable.hpp"
#include "objects/inputmethods/flatscreenpointer.hpp"
#include "objects/inputmethods/skhand.hpp"
using namespace StardustXRServer;

//Stardust XR Integrations includes
#include "integrations/sk_internal_defs.hpp"
#include "integrations/wayland/wayland.hpp"
#include <EGL/eglext.h>

// StereoKit includes
#include <stereokit.h>
#include <stereokit_ui.h>
using namespace sk;
std::atomic<uint64_t> frame = {0};

// Third party local includes
#include "CLI11.hpp"
#include "XdgUtils/BaseDir/BaseDir.h"

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
ClientManager *clientManager = new ClientManager();
Client *serverInternalClient = new Client(*clientManager, 0);

// Builtin inputs
TypedNodeRef<FlatscreenPointer> flatscreenPointer;
std::array<TypedNodeRef<SKHandInput>, 2> stereokitHands;

// Wayland global variables
Wayland *wayland = nullptr;

void shutdown(int signal) {
	sk_quit();
	printf("Shutting down Stardust\n");
}

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

	signal(SIGINT, shutdown);
	signal(SIGSTOP, shutdown);
	signal(SIGQUIT, shutdown);

	tex_t skytex;
	FILE *skyfile = fopen((XdgUtils::BaseDir::XdgConfigHome()+"/stardust/skytex.hdr").c_str(), "ro");
	if(!skyfile) {
		gradient_t skycolor = gradient_create();
		gradient_add(skycolor, {0, 0, 0, 1}, 0.0f);
		gradient_add(skycolor, {0, 0, 0, 1}, 1.0f);
		skytex = tex_gen_cubemap(skycolor, vec3_up, 2);
	} else {
		fclose(skyfile);
		spherical_harmonics_t skylight;
		skytex = tex_create_cubemap_file((XdgUtils::BaseDir::XdgConfigHome()+"/stardust/skytex.hdr").c_str(), true, &skylight);
		render_set_skylight(skylight);
	}
	render_set_skytex(skytex);

	struct skg_platform_data_t stereokitPlatformData = skg_get_platform_data();
	wayland = new Wayland(stereokitPlatformData._egl_display, stereokitPlatformData._egl_context, EGL_PLATFORM_GBM_MESA);

	// Set up debugging
	if(args.fieldDebug)
		debugSetup();

	ui_enable_far_interact(false);
	if(args.flatscreen) { // Add the flatscreen pointer if we're in flatscreen mode
		input_hand_visible(handed_left, false);
		input_hand_visible(handed_right, false);
		flatscreenPointer = new FlatscreenPointer(serverInternalClient);
		serverInternalClient->scenegraph.addNode("/test/flatscreenpointer", flatscreenPointer);
	} else { // Add the StereoKit hand representation if we're not in flatscreen
		stereokitHands[0] = new SKHandInput(serverInternalClient, handed_left);
		stereokitHands[1] = new SKHandInput(serverInternalClient, handed_right);
		serverInternalClient->scenegraph.addNode("/test/skhandleft", stereokitHands[0]);
		serverInternalClient->scenegraph.addNode("/test/skhandright", stereokitHands[1]);
	}

	// Start the startup script
	int pid = fork();
	if(pid == 0)
		execlp((XdgUtils::BaseDir::XdgConfigHome()+"/stardust/startup").c_str(), "", nullptr);

	// Every stereokit step
	while (sk_step([]() {
		// Handle disconnected clients before anything else to ensure scenegraph is clean
		clientManager->handleDisconnectedClients();
		clientManager->handleNewlyConnectedClients();

        // Delete any nodes that are queued up to delete
        Node::destroyNodes();

		// Update environment settings
		DrawableInterface::updateEnvironment();

		// Process the zones
		SpatialInterface::updateZones();

		// Accept items
		ItemInterface::updateItems();

		// Update wayland
		if(wayland)
			wayland->update();

		//Propagate the update and draw methods on scenegraph nodes
		Drawable::drawAll();

		// Propagate the debug draw methods if the appropriate attribute is set
		if(args.fieldDebug)
			clientManager->callClientsDebug();
		
		//Increment the frame count
		frame++;

		// Send logicStep signals to clients
		RootInterface::sendLogicStepSignals();

		// Process all the input and send it to the clients
	   if(flatscreenPointer)
		   flatscreenPointer.ptr()->update();
	   if(stereokitHands[0])
		   stereokitHands[0].ptr()->update();
	   if(stereokitHands[1])
		   stereokitHands[1].ptr()->update();
		InputInterface::processInput();
	})) {}

	delete serverInternalClient;
	if(wayland)
		delete wayland;
	delete clientManager;

	sk_shutdown();
	return 0;
}
