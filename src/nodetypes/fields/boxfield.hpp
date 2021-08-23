#pragma once

#include "field.hpp"

namespace StardustXRServer {

class BoxField : public Field {
public:
	BoxField(Client *client, Spatial *spatialParent, vec3 position, quat rotation, vec3 size);
	// virtual ~BoxField();

	vec3 size;

	std::vector<uint8_t> setSize(flexbuffers::Reference data, bool);

	virtual float localDistance(const vec3 point);

	void debug();
};

} // namespace StardustXRServer
