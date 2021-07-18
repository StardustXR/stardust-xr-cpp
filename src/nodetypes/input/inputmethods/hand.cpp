#include "hand.hpp"

#include <cmath>
#include <limits>

using namespace std;

namespace StardustXRServer {

HandInput::HandInput(Client *client) : InputMethod(client) {
	translatable = true;
	rotatable = true;
	scalable = false;
}
HandInput::~HandInput() {}

joint HandInput::localSKToSpaceFlexJoint(Spatial *space, hand_joint_t joint) {
	return {
		skToFlex(localToSpacePoint(space, joint.position)),
		skToFlex(localToSpaceRotation(space, joint.orientation)),
		joint.radius
	};
}

void HandInput::updateSpaceFlexJoint(Spatial *space, hand_joint_t joint, struct joint *updateJoint) {
	struct joint localJoint = localSKToSpaceFlexJoint(space, joint);

	updateJoint->mutable_position().mutate_x(localJoint.position().x());
	updateJoint->mutable_position().mutate_y(localJoint.position().y());
	updateJoint->mutable_position().mutate_z(localJoint.position().z());

	updateJoint->mutable_rotation().mutate_w(localJoint.rotation().w());
	updateJoint->mutable_rotation().mutate_x(localJoint.rotation().x());
	updateJoint->mutable_rotation().mutate_y(localJoint.rotation().y());
	updateJoint->mutable_rotation().mutate_z(localJoint.rotation().z());

	updateJoint->mutate_radius(localJoint.radius());
}

float HandInput::distanceTo(InputHandler *handler) {
	float minDistance = numeric_limits<float>::max();
	for(uint i=0; i<fingerJointCount; ++i) {
		float distance = handler->field->distance(this, fingerJoints[i].position);
		minDistance = min(minDistance, distance);
	}
	return minDistance;
}

InputDataRaw HandInput::type() {
	return InputDataRaw_Hand;
}

flatbuffers::Offset<void> HandInput::generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler) {
	std::vector<joint> relativeFingerJoints(fingerJointCount);
	for(uint i=0; i<fingerJointCount; ++i)
		relativeFingerJoints[i] = localSKToSpaceFlexJoint(handler, fingerJoints[i]);

	joint flexPalmJoint = localSKToSpaceFlexJoint(handler, palmJoint);
	joint flexWristJoint = localSKToSpaceFlexJoint(handler, wristJoint);
	joint flexElbowJoint = localSKToSpaceFlexJoint(handler, elbowJoint);

	return CreateHandDirect(
		fbb,
		&relativeFingerJoints,
		&flexPalmJoint,
		&flexWristJoint,
		hasForearm ? &flexElbowJoint : nullptr
	).Union();
}

void HandInput::updateInput(InputData *data, Spatial *space) {
	StardustXR::Hand *handInput = static_cast<StardustXR::Hand *>(data->mutable_input());
	for(uint i=0; i<fingerJointCount; ++i) {
		updateSpaceFlexJoint(space, fingerJoints[i], handInput->mutable_finger_joints()->GetMutableObject(i));
	}
	updateSpaceFlexJoint(space, palmJoint, handInput->mutable_palm());
	updateSpaceFlexJoint(space, wristJoint, handInput->mutable_wrist());
	if(hasForearm)
		updateSpaceFlexJoint(space, elbowJoint, handInput->mutable_elbow());
}


} // namespace StardustXRServer
