// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_COMMON_STARDUSTXR_H_
#define FLATBUFFERS_GENERATED_COMMON_STARDUSTXR_H_

#include "flatbuffers/flatbuffers.h"

namespace StardustXR {

struct vec3;

struct quat;

struct pose;

struct joint;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  vec3()
      : x_(0),
        y_(0),
        z_(0) {
  }
  vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  void mutate_x(float _x) {
    flatbuffers::WriteScalar(&x_, _x);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  void mutate_y(float _y) {
    flatbuffers::WriteScalar(&y_, _y);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
  void mutate_z(float _z) {
    flatbuffers::WriteScalar(&z_, _z);
  }
};
FLATBUFFERS_STRUCT_END(vec3, 12);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) quat FLATBUFFERS_FINAL_CLASS {
 private:
  float w_;
  float x_;
  float y_;
  float z_;

 public:
  quat()
      : w_(0),
        x_(0),
        y_(0),
        z_(0) {
  }
  quat(float _w, float _x, float _y, float _z)
      : w_(flatbuffers::EndianScalar(_w)),
        x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float w() const {
    return flatbuffers::EndianScalar(w_);
  }
  void mutate_w(float _w) {
    flatbuffers::WriteScalar(&w_, _w);
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  void mutate_x(float _x) {
    flatbuffers::WriteScalar(&x_, _x);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  void mutate_y(float _y) {
    flatbuffers::WriteScalar(&y_, _y);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
  void mutate_z(float _z) {
    flatbuffers::WriteScalar(&z_, _z);
  }
};
FLATBUFFERS_STRUCT_END(quat, 16);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) pose FLATBUFFERS_FINAL_CLASS {
 private:
  StardustXR::vec3 position_;
  StardustXR::quat rotation_;

 public:
  pose()
      : position_(),
        rotation_() {
  }
  pose(const StardustXR::vec3 &_position, const StardustXR::quat &_rotation)
      : position_(_position),
        rotation_(_rotation) {
  }
  const StardustXR::vec3 &position() const {
    return position_;
  }
  StardustXR::vec3 &mutable_position() {
    return position_;
  }
  const StardustXR::quat &rotation() const {
    return rotation_;
  }
  StardustXR::quat &mutable_rotation() {
    return rotation_;
  }
};
FLATBUFFERS_STRUCT_END(pose, 28);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) joint FLATBUFFERS_FINAL_CLASS {
 private:
  StardustXR::vec3 position_;
  StardustXR::quat rotation_;
  float radius_;

 public:
  joint()
      : position_(),
        rotation_(),
        radius_(0) {
  }
  joint(const StardustXR::vec3 &_position, const StardustXR::quat &_rotation, float _radius)
      : position_(_position),
        rotation_(_rotation),
        radius_(flatbuffers::EndianScalar(_radius)) {
  }
  const StardustXR::vec3 &position() const {
    return position_;
  }
  StardustXR::vec3 &mutable_position() {
    return position_;
  }
  const StardustXR::quat &rotation() const {
    return rotation_;
  }
  StardustXR::quat &mutable_rotation() {
    return rotation_;
  }
  float radius() const {
    return flatbuffers::EndianScalar(radius_);
  }
  void mutate_radius(float _radius) {
    flatbuffers::WriteScalar(&radius_, _radius);
  }
};
FLATBUFFERS_STRUCT_END(joint, 32);

}  // namespace StardustXR

#endif  // FLATBUFFERS_GENERATED_COMMON_STARDUSTXR_H_