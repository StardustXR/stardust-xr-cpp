#pragma once

#include "field.hpp"

namespace StardustXRServer {

class BoxField : public Field {
public:
	BoxField(Client *client, Spatial *spatialParent, sk::pose_t transform, sk::vec3 size);
	// virtual ~BoxField();

	sk::vec3 size;

	std::vector<uint8_t> setSize(Client *callingClient, flexbuffers::Reference data, bool);

	virtual float localDistance(const sk::vec3 point);

	void debug();
};

} // namespace StardustXRServer
