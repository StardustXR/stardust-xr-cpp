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
#include "core/eventloop.hpp"
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
struct CLIArgs {
	bool flatscreen = false;
	bool fieldDebug = false;
	bool disableWayland = false;

	int parse(int argc, const char* const argv[]) {
		CLI::App app("Stardust XR");
		app.add_flag("-F,--flatscreen", flatscreen, "Run Stardust in flatscreen mode");
		app.add_flag("--field-debug", fieldDebug, "Draw translucent meshes around fields");
		app.add_flag("--disable-wayland", disableWayland, "Disable wayland compositor component");
		try {
			(app).parse((argc), (argv));
		} catch(const CLI::ParseError &e) {
			return (app).exit(e);
		}
		return -1;
	}
};
CLIArgs args = {};
extern void debugSetup();

// Basic internals
EventLoop *eventLoop = nullptr;
Node *serverInternalNode = nullptr;
Wayland *wayland = nullptr;

// Builtin inputs
FlatscreenPointer *flatscreenPointer;
std::array<SKHandInput *, 2> stereokitHands;

void shutdown(int signal) {
	sk_quit();
}

int main(int argc, char *argv[]) {
	int parse_result = args.parse(argc, argv);
	if (parse_result != -1) return parse_result;

	sk_settings_t settings = {
		.app_name = args.flatscreen ? "Stardust XR (Flatscreen)" : "Stardust XR",
		.assets_folder = "",
		.display_preference = args.flatscreen ? display_mode_flatscreen : display_mode_mixedreality,
//		.log_filter = log_diagnostic,
		.log_filter = log_warning,
	};
	if(!sk_init(settings))
		exit(1);

	signal(SIGINT, shutdown);

	bool eventLoopSetupSuccessful = false;
	eventLoop = new EventLoop(eventLoopSetupSuccessful);
	if(!eventLoopSetupSuccessful) {
		delete eventLoop;
		sk_shutdown();
		perror("Event loop setup failed");
		return 1;
	}
	serverInternalNode = new Node(nullptr, false);

	if(!args.disableWayland) {
		struct skg_platform_data_t stereokitPlatformData = skg_get_platform_data();
		wayland = new Wayland(stereokitPlatformData._egl_display, stereokitPlatformData._egl_context);
	}

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

	// Set up debugging
	if(args.fieldDebug)
		debugSetup();

	ui_enable_far_interact(false);
	if(args.flatscreen) { // Add the flatscreen pointer if we're in flatscreen mode
		input_hand_visible(handed_left, false);
		input_hand_visible(handed_right, false);
		flatscreenPointer = new FlatscreenPointer(nullptr, 0.05f);
		serverInternalNode->addChild("flatscreenPointer", flatscreenPointer);
	} else { // Add the StereoKit hand representation if we're not in flatscreen
		stereokitHands[0] = new SKHandInput(nullptr, handed_left);
		serverInternalNode->addChild("leftHand", stereokitHands[0]);
		stereokitHands[1] = new SKHandInput(nullptr, handed_right);
		serverInternalNode->addChild("rightHand", stereokitHands[1]);
	}

	// Start the startup script
	if(wayland) {
		setenv("GDK_BACKEND", "wayland", true);
		setenv("QT_QPA_PLATFORM", "wayland", true);
	}
	int pid = fork();
	if(pid == 0)
		execl((XdgUtils::BaseDir::XdgConfigHome()+"/stardust/startup").c_str(), "", nullptr);

	// Every stereokit step
	while (sk_step([]() {
        // Delete any nodes that are queued up to delete
		Node::destroyNodes();

		// Update environment settings
		DrawableInterface::updateEnvironment();

		// Process the zones
		Zone::updateZones();

		// Accept items
		ItemInterface::updateItems();

		//Propagate the update and draw methods on scenegraph nodes
		Drawable::drawAll();

		// Propagate the debug draw methods if the appropriate attribute is set
		if(args.fieldDebug)
			eventLoop->callClientsDebug();
		
		//Increment the frame count
		frame++;

		// Send logicStep signals to clients
		RootInterface::sendLogicStepSignals();

		// Process all the input and send it to the clients
		if(args.flatscreen) {
			flatscreenPointer->update();
		} else {
			stereokitHands[0]->update();
			stereokitHands[1]->update();
		}
		InputInterface::processInput();

		// Handle all the wayland events!
		if(wayland && !wayland->dispatch())
		   sk_quit();
	})) {}
	printf("Shutting down Stardust\n");

	delete serverInternalNode;
	if(wayland)
		delete wayland;
	delete eventLoop;

	sk_shutdown();
	return 0;
}
