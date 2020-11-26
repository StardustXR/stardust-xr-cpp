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

	std::vector<uint8_t> setPosition(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setRotation(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setScale(uint sessionID, flexbuffers::Reference data, bool returnValue);

	std::vector<uint8_t> setPose(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setTransform(uint sessionID, flexbuffers::Reference data, bool returnValue);

	vec3 position = vec3_zero;
	quat rotation = quat_identity;
	vec3 scale = vec3_one;

	void transformDirty() { transformMatrixDirty = true; }
protected:
	bool transformMatrixDirty = false;
	matrix transform = matrix_identity;

	void cleanTransform();
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_SPATIALNODE_HPP
