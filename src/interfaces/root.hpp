#pragma once

#include "../nodetypes/core/callback.hpp"
#include "../nodetypes/spatial/spatial.hpp"

#include <mutex>
#include <vector>

namespace StardustXRServer {

class RootInterface : public Spatial {
public:
	RootInterface(Client *client);
	~RootInterface();

	static void sendLogicStepSignals();
	std::vector<uint8_t> subscribeLogicStep(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> disconnect(Client *callingClient, flexbuffers::Reference data, bool returnValue);

protected:
	static void logicStepSignal(uint32_t index, Callback &updateMethod);
	static std::mutex updateMethodsMutex;
	static std::vector<Callback> updateMethods;

	static double prevFrameTime;
	static double frameTime;
	static double delta;
};

} // namespace StardustXRServer
