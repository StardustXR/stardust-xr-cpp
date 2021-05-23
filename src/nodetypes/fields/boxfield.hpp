#pragma once

#include "field.hpp"

namespace StardustXRServer {

class BoxField : public Field {
public:
	BoxField(vec3 size);
	virtual ~BoxField();

	vec3 size;

	virtual float localDistance(const vec3 point);

	void update();

private:
	mesh_t debugMesh = nullptr;
	model_t debugModel = nullptr;
};

} // namespace StardustXRServer
