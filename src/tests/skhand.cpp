#include "skhand.hpp"

using namespace sk;

namespace StardustXRServer {

SKHandInput::SKHandInput(handed_ handed) {
	this->handed = handed;
	this->hand = input_hand(handed);
	this->hasForearm = false;
}
SKHandInput::~SKHandInput() {}

void SKHandInput::update() {
	// Transfer stereokit hand fingers to HandInput fingers
	for(uint f=0; f<5; ++f) {
		bool isThumb = f == 0;
		for(uint b=isThumb; b<5; ++b) {
			fingerJoints[(5*f)+b-1] = hand->fingers[f][b];
		}
	}
	palmJoint = {
		hand->fingers[2][0].position + hand->fingers[2][1].position * 0.5f, // Average the positions to get the middle of the bone (OpenXR spec)
		hand->fingers[2][0].orientation, // Same orientation as the metacarpal (OpenXR spec)
		hand->fingers[2][0].radius + hand->fingers[2][1].radius * 0.5f // Average the radii
	};
	wristJoint = {
		hand->wrist.position,
		hand->wrist.orientation,
		vec3_magnitude(hand->fingers[0][0].position - hand->fingers[4][0].position) * 0.5f
	};

	datamap["isLeft"] = handed == handed_left;
	datamap["pinchDistance"] = vec3_magnitude(hand->fingers[0][4].position - hand->fingers[1][4].position);
	datamap["pinchStrength"] = hand->pinch_state & button_state_active;
}

} // namespace StardustXRServer
