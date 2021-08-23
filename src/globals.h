#pragma once

#include <stereokit.h>
#include <cstdint>
#include "core/clientmanager.hpp"

extern StardustXRServer::ClientManager clientManager;

struct CLIArgs {
	bool flatscreen = false;
	bool fieldDebug = false;
	int parse(int argc, const char* const argv[]);
};
extern CLIArgs args;

extern uint64_t frame;

extern sk::material_t fieldDebugMat;
extern sk::mesh_t sphereFieldMesh;
extern sk::mesh_t boxFieldMesh;
