#pragma once

#include "../nodetypes/core/node.hpp"

#include <mutex>
#include <vector>

namespace StardustXRServer {

typedef struct {
	Client *client;
	std::string nodePath;
	std::string methodName;
} LifeCycleUpdateMethod;

class RootInterface : public Node {
public:
	RootInterface(Client *client);
	~RootInterface();

	static void sendLogicStepSignals();
	std::vector<uint8_t> subscribeLogicStep(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> disconnect(flexbuffers::Reference data, bool returnValue);

protected:
	static void logicStepSignal(uint32_t index, LifeCycleUpdateMethod &updateMethod);
	static std::mutex updateMethodsMutex;
	static std::vector<LifeCycleUpdateMethod> updateMethods;

	static double prevFrameTime;
	static double frameTime;
	static double delta;
};

} // namespace StardustXRServer
