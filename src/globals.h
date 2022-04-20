#pragma once

#include "nodetypes/core/node.hpp"
#include <stereokit.h>
#include <cstdint>
#include <atomic>

extern std::atomic<uint64_t> frame;

extern sk::material_t fieldDebugMat;
extern sk::mesh_t boxFieldMesh;
extern sk::mesh_t cylinderFieldMesh;
extern sk::mesh_t sphereFieldMesh;

extern StardustXRServer::Node *serverInternalNode;
