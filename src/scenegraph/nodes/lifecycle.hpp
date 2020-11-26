#ifndef STARDUSTXRSERVER_LIFECYCLEINTERFACE_HPP
#define STARDUSTXRSERVER_LIFECYCLEINTERFACE_HPP

#include "../../nodetypes/node.hpp"
#include <list>

namespace StardustXRServer {

class LifeCycleInterface : public Node {
public:
	LifeCycleInterface();
	virtual ~LifeCycleInterface() {}

	void sendLogicStepSignals();

	std::vector<uint8_t> subscribeLogicStep(uint sessionID, flexbuffers::Reference data, bool returnValue);

protected:
	typedef struct {
		uint sessionID;
		std::string nodePath;
		std::string methodName;
	} LifeCycleUpdateMethod;

	std::list<LifeCycleUpdateMethod> lifeCycleUpdateMethodList;

	double prevFrameTime;
	double frameTime;

};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_LIFECYCLEINTERFACE_HPP
