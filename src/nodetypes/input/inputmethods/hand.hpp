#pragma once

#include "../inputmethod.hpp"
#include "../../spatial.hpp"
using namespace sk;

namespace StardustXRServer {

class HandInput : public InputMethod {
public:
	HandInput(Client *client);
	virtual ~HandInput();

	static constexpr uint fingerJointCount = 25;
	hand_joint_t fingerJoints[fingerJointCount];
	hand_joint_t palmJoint;
	hand_joint_t wristJoint;
	bool hasForearm = false;
	hand_joint_t elbowJoint;
	joint localSKToSpaceFlexJoint(Spatial *space, hand_joint_t joint);
	void updateSpaceFlexJoint(Spatial *space, hand_joint_t joint, struct joint *updateJoint);

	float distanceTo(InputHandler *handler);
	InputDataRaw type();
	flatbuffers::Offset<void> generateInput(flatbuffers::FlatBufferBuilder &fbb, InputHandler *handler);
	void updateInput(InputData *data, Spatial *space);
protected:
	sk::vec3 flexToSK(StardustXR::vec3 flex) {
		return {
			flex.x(),
			flex.y(),
			flex.z()
		};
	}
	sk::quat flexToSK(StardustXR::quat flex) {
		return {
			flex.w(),
			flex.x(),
			flex.y(),
			flex.z()
		};
	}
	hand_joint_t flexToSK(joint flex) {
		return {
			flexToSK(flex.position()),
			flexToSK(flex.rotation()),
			flex.radius()
		};
	}

	StardustXR::vec3 skToFlex(sk::vec3 sk) {
		return {
			sk.x,
			sk.y,
			sk.z
		};
	}
	StardustXR::quat skToFlex(sk::quat sk) {
		return {
			sk.w,
			sk.x,
			sk.y,
			sk.z
		};
	}
	joint skToFlex(hand_joint_t sk) {
		return {
			skToFlex(sk.position),
			skToFlex(sk.orientation),
			sk.radius
		};
	}
};

}
