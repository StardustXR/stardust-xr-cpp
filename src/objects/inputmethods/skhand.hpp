#pragma once

#include "../../nodetypes/input/inputmethods/hand.hpp"

namespace StardustXRServer {

class SKHandInput : public HandInput {
public:
	SKHandInput(Client *client, handed_ handed);
	virtual ~SKHandInput();

	void update();
	void serializeData(flexbuffers::Builder &fbb);
protected:
	handed_ handed = handed_left;
	const hand_t *hand;
};

} // namespace StardustXRServer
