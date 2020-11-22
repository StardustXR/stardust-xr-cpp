#ifndef STARDUSTXR_MODEL_NODE_H
#define STARDUSTXR_MODEL_NODE_H

#include "../drawablenode.hpp"
#include <stereokit.h>

namespace StardustXRServer {

class ModelNode : public DrawableNode {
public:
	ModelNode();
	virtual ~ModelNode() {}

	void update();
	void draw();
protected:
	sk::mesh_t mesh;
	sk::material_t material;
};

}


#endif // STARDUSTXR_MODEL_NODE_H
