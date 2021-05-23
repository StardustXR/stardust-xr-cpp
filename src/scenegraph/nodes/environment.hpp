#pragma once

#include <stereokit.h>
#include "../../nodetypes/node.hpp"

namespace StardustXRServer {

class EnvironmentInterface : public Node {
public:
	EnvironmentInterface();
	virtual ~EnvironmentInterface() {}

	void update();

	std::vector<uint8_t> visible(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setSkytex(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setLighting(uint sessionID, flexbuffers::Reference data, bool returnValue);

private:
	bool skytexDisableLatch = true;

	sk::tex_t skytex;
	sk::spherical_harmonics_t lighting;
};

} // namespace StardustXRServer