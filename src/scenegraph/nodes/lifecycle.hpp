#pragma once

#include "../../nodetypes/node.hpp"
#include "../../util/threadsafelist.hpp"

namespace StardustXRServer {

class LifeCycleInterface : public Node {
public:
	LifeCycleInterface();
	virtual ~LifeCycleInterface() {}

	void sendLogicStepSignals();
	void handleMessengerDeletion(uint sessionID);

	std::vector<uint8_t> subscribeLogicStep(uint sessionID, flexbuffers::Reference data, bool returnValue);

protected:
	typedef struct {
		uint sessionID;
		std::string nodePath;
		std::string methodName;
	} LifeCycleUpdateMethod;

	void logicStepSignal(uint32_t index, LifeCycleUpdateMethod &updateMethod);
	ThreadSafeList<LifeCycleUpdateMethod> lifeCycleUpdateMethodList;

	double prevFrameTime;
	double frameTime;
	double delta;
};

} // namespace StardustXRServer
