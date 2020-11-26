#ifndef STARDUSTXRSERVER_SPATIALNODE_HPP
#define STARDUSTXRSERVER_SPATIALNODE_HPP

#include "node.hpp"
#include <stereokit.h>

using namespace sk;

namespace StardustXRServer {

class SpatialNode : public Node
{
public:
	SpatialNode();
	virtual ~SpatialNode() {}

	std::vector<uint8_t> setPosition(flexbuffers::Reference &data, bool returnValue);

	vec3 position = vec3_zero;
	quat orientation = quat_identity;
	vec3 scale = vec3_one;

protected:
	bool transformMatrixDirty = false;
	matrix transform = matrix_identity;
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_SPATIALNODE_HPP
