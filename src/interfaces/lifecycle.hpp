#pragma once

#include "../nodetypes/node.hpp"

#include <mutex>
#include <vector>

namespace StardustXRServer {

typedef struct {
	Client *client;
	std::string nodePath;
	std::string methodName;
} LifeCycleUpdateMethod;

class LifeCycleInterface : public Node {
public:
	LifeCycleInterface(Client *client);
	~LifeCycleInterface();

	static void sendLogicStepSignals();

	std::vector<uint8_t> subscribeLogicStep(flexbuffers::Reference data, bool returnValue);

protected:
	static void logicStepSignal(uint32_t index, LifeCycleUpdateMethod &updateMethod);
	static std::mutex updateMethodsMutex;
	static std::vector<LifeCycleUpdateMethod> updateMethods;

	static double prevFrameTime;
	static double frameTime;
	static double delta;
};

} // namespace StardustXRServer
