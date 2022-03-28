#include "flex.hpp"

using namespace sk;

sk::vec2 FlexToSKVec2(flexbuffers::TypedVector flexVec) {
	return vec2 {
		flexVec[0].AsFloat(),
		flexVec[1].AsFloat()
	};
}

sk::vec3 FlexToSKVec3(flexbuffers::TypedVector flexVec) {
	return vec3 {
		flexVec[0].AsFloat(),
		flexVec[1].AsFloat(),
		flexVec[2].AsFloat()
	};
}

sk::quat FlexToSKQuat(flexbuffers::TypedVector flexVec) {
	return quat {
		flexVec[0].AsFloat(),
		flexVec[1].AsFloat(),
		flexVec[2].AsFloat(),
		flexVec[3].AsFloat()
	};
}

sk::pose_t FlexToSKPose(flexbuffers::TypedVector pos, flexbuffers::TypedVector rot) {
	return pose_t {
		FlexToSKVec3(pos),
		FlexToSKQuat(rot)
	};
}

matrix FlexToSKTransform(flexbuffers::TypedVector pos, flexbuffers::TypedVector rot, flexbuffers::TypedVector scl) {
	return matrix_trs(FlexToSKVec3(pos), FlexToSKQuat(rot), FlexToSKVec3(scl));
}

sk::color128 FlexToSKColor(flexbuffers::TypedVector flexVec) {
	return color128 {
		flexVec[0].AsFloat(),
		flexVec[1].AsFloat(),
		flexVec[2].AsFloat(),
		flexVec[3].AsFloat()
	};
}
