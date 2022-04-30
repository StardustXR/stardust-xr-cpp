#include "hmd.hpp"

using namespace sk;

namespace StardustXRServer {

HMD::HMD() : Spatial(nullptr, matrix_identity) {}

void HMD::update() {
	setTransformMatrix(pose_matrix(*input_head()));
}

}
