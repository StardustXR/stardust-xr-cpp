#ifndef STARDUSTXR_SKYBOXINTERFACE_H
#define STARDUSTXR_SKYBOXINTERFACE_H

#include "../../nodetypes/node.hpp"

namespace StardustXRServer {

class SkyboxInterface : public Node {
public:
	SkyboxInterface();
	virtual ~SkyboxInterface() {}

	void update();

private:
	bool skytexEnabled = true;
};

} // namespace StardustXRServer

#endif // STARDUSTXR_SKYBOXINTERFACE_H
