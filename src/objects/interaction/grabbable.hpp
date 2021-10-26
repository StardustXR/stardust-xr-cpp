#pragma once

#include <stereokit.h>
#include "xinteract.hpp"

#include "../../nodetypes/fields/field.hpp"
#include "../../nodetypes/input/inputhandler.hpp"
#include "../../nodetypes/input/inputmethods/hand.hpp"
#include "../../nodetypes/input/inputmethods/pointer.hpp"

namespace StardustXRServer {

class Grabbable : public InputHandler {
public:
	explicit Grabbable(Client *client, sk::vec3 origin, sk::quat orientation, Field &field, float maxDistance = 0.01f);
	~Grabbable();

	void update();

	bool isActive();
	bool activeChanged();

	void sendInput(uint64_t oldFrame, std::list<DistanceLink> distanceLinks, std::vector<uint8_t> &inputData);

	float maxDistance;
protected:
	bool handInput(HandInput *hand);
	bool pointerInput(PointerInput *pointer);
	void grab(sk::matrix grabMat);

	XInteract xInteract;
	sk::matrix startItemMat;
	sk::matrix startGrabMat;
	float pinchDistance = 0.0f;
	float pointDistance = 0.0f;

	Field *field;
};

}
