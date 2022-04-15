#include "skhand.hpp"

using namespace sk;

namespace StardustXRServer {

SKHandInput::SKHandInput(Client *client, handed_ handed) : HandInput(client) {
	this->handed = handed;
	this->hand = input_hand(handed);
	this->hasForearm = false;
}
SKHandInput::~SKHandInput() {}

void SKHandInput::update() {
	// Transfer stereokit hand fingers to HandInput fingers
	for(uint f=0; f<5; ++f) {
		for(uint b=0; b<5; ++b) {
			fingerJoints[(5*f)+b] = hand->fingers[f][b];
		}
	}
	palmJoint = {
		hand->palm.position,
		hand->palm.orientation,
		hand->fingers[2][0].radius + hand->fingers[2][1].radius * 0.5f // Average the radii
	};
	wristJoint = {
		hand->wrist.position,
		hand->wrist.orientation,
		vec3_magnitude(hand->fingers[0][0].position - hand->fingers[4][0].position) * 0.5f
	};
}

void SKHandInput::serializeData(flexbuffers::Builder &fbb) {
	HandInput::serializeData(fbb);
	fbb.Bool("isLeft", handed == handed_left);
	fbb.Float("pinchDistance", vec3_magnitude(hand->fingers[0][4].position - hand->fingers[1][4].position));
	fbb.Float("pinchStrength", hand->pinch_state & button_state_active);
	fbb.Float("grabStrength", hand->grip_state & button_state_active);
}

} // namespace StardustXRServer
