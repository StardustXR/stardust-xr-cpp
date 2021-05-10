#pragma once

#include "../nodetypes/input/inputmethods/hand.hpp"

namespace StardustXRServer {

class SKHandInput : public HandInput {
public:
	SKHandInput(handed_ handed);
	virtual ~SKHandInput();

	void update();
protected:
	handed_ handed = handed_left;
	const hand_t *hand;
};

} // namespace StardustXRServer