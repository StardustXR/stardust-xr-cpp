#pragma once

#include <stereokit.h>
#include "core/clientmanager.hpp"

extern StardustXRServer::ClientManager clientManager;

struct CLIArgs {
	bool flatscreen = false;
	bool fieldDebug = false;
	int parse(int argc, const char* const argv[]);
};
extern CLIArgs args;

extern sk::material_t fieldDebugMat;
