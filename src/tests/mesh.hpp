#ifndef STARDUSTXR_TEST_MESH_NODE_H
#define STARDUSTXR_TEST_MESH_NODE_H

#include "../nodetypes/drawablenode.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class TestMeshNode : public DrawableNode {
public:
	TestMeshNode();
	virtual ~TestMeshNode() {}

	void update();
	void draw();
protected:
	sk::mesh_t mesh;
	sk::material_t material;
};

} //namespace StardustXRServer

#endif //STARDUSTXR_TEST_MESH_NODE_H
