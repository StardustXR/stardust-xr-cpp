#pragma once

#include "../../nodetypes/input/inputmethods/pointer.hpp"
#include "../../nodetypes/data/receiver.hpp"
#include "../../nodetypes/data/sender.hpp"
#include <stardustxr/common/flex.hpp>

namespace StardustXRServer {

class FlatscreenPointer : public PointerInput {
public:
	FlatscreenPointer(Client *client, float keySendDistance);

	void update();
	void serializeData(flexbuffers::Builder &fbb);
private:
	float keySendDistance;
	std::vector<uint8_t> keyMask;
	PulseSender keySender;
	void sendKeyEvents(Registry<PulseReceiver> &receivers, StardustXR::ArgsConstructor args);

	uint8_t shiftIndex;
	uint8_t capsIndex;
	uint8_t ctrlIndex;
	uint8_t altIndex;
};

} // namespace StardustXRServer
