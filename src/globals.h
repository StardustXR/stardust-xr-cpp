#pragma once

#include "core/client.hpp"
#include <stereokit.h>
#include <cstdint>

struct CLIArgs {
	bool flatscreen = false;
	bool fieldDebug = false;
	int parse(int argc, const char* const argv[]);
};
extern CLIArgs args;

extern uint64_t frame;

extern sk::material_t fieldDebugMat;
extern sk::mesh_t boxFieldMesh;
extern sk::mesh_t cylinderFieldMesh;
extern sk::mesh_t sphereFieldMesh;

extern StardustXRServer::Client serverInternalClient;
