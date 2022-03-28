#pragma once

#include "stereokit.h"
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/flexbuffers.h>

sk::vec2   FlexToSKVec2(flexbuffers::TypedVector vec);
sk::vec3   FlexToSKVec3(flexbuffers::TypedVector vec);
sk::quat   FlexToSKQuat(flexbuffers::TypedVector vec);
sk::pose_t FlexToSKPose(flexbuffers::TypedVector pos, flexbuffers::TypedVector rot);
sk::matrix FlexToSKTransform(flexbuffers::TypedVector pos, flexbuffers::TypedVector rot, flexbuffers::TypedVector scl);
