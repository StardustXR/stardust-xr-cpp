#include "hmd.hpp"

using namespace sk;

namespace StardustXRServer {

HMD::HMD(Client *client) :
Spatial(client, matrix_identity) {
	enabled = false;
}
void HMD::update() {
	setTransformMatrix(matrix_trs(input_head()->position, input_head()->orientation));
}

} // namespace StardustXRServer
