#include "spatialnode.hpp"

namespace StardustXRServer {

SpatialNode::SpatialNode() {

}

std::vector<uint8_t> SpatialNode::setPosition(flexbuffers::Reference &data, bool returnValue) {
	flexbuffers::TypedVector vector = data.AsTypedVector();
	position = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };
	transformMatrixDirty = true;

	return FlexbufferFromArguments([](flexbuffers::Builder &fbb) { fbb.Null(); });
}

} // namespace StardustXRServer
