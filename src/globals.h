#pragma once

#include <stereokit.h>
#include "scenegraph/scenegraph.hpp"
#include <stardustxr/server/messengermanager.hpp>

extern StardustXRServer::Scenegraph scenegraph;
extern StardustXR::MessengerManager messengerManager;

struct CLIArgs {
	bool flatscreen = false;
	bool fieldDebug = false;
	int parse(int argc, const char* const argv[]);
};
extern CLIArgs args;

extern sk::material_t fieldDebugMat;
