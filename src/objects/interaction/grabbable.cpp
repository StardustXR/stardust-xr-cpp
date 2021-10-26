#include "grabbable.hpp"
#include "xinteract.hpp"

#include "../../interfaces/input.hpp"
#include "../../globals.h"

using namespace sk;

namespace StardustXRServer {

Grabbable::Grabbable(Client *client, sk::vec3 origin, sk::quat orientation, Field &field, float maxDistance) :
		InputHandler(client, nullptr, vec3_zero, quat_identity, &field, "", "") {

//	inputHandler.handHandlerMethod = std::bind(&Grabbable::handInput, this, std::placeholders::_1, std::placeholders::_2);
//	inputHandler.pointerHandlerMethod = std::bind(&Grabbable::pointerInput, this, std::placeholders::_1, std::placeholders::_2);

	this->maxDistance = maxDistance;
	this->field = &field;

	std::lock_guard<std::mutex> lock(InputInterface::inputVectorsMutex);
	InputInterface::inputHandlers.push_back(this);
}
Grabbable::~Grabbable() {
	std::lock_guard<std::mutex> lock(InputInterface::inputVectorsMutex);
	InputInterface::inputHandlers.erase(std::remove(InputInterface::inputHandlers.begin(), InputInterface::inputHandlers.end(), this));
}

void Grabbable::update() {
	if(xInteract.hasActiveChanged())
		zoneable = !xInteract.isActive();
	xInteract.update();
}

bool Grabbable::isActive() {
	return xInteract.isActive();
}

bool Grabbable::activeChanged() {
	return xInteract.hasActiveChanged();
}

void Grabbable::sendInput(uint64_t oldFrame, std::list<DistanceLink> distanceLinks, std::vector<uint8_t> &inputData) {
	if(oldFrame < frame)
		return;

	bool capture = false;

	DistanceLink link = *distanceLinks.begin();
	PointerInput *pointer = dynamic_cast<PointerInput *>(link.method);
	HandInput *hand = dynamic_cast<HandInput *>(link.method);
	if(pointer != nullptr)
		capture = pointerInput(pointer);
	else if(hand != nullptr)
		capture = handInput(hand);

	distanceLinks.pop_front();
	if(distanceLinks.begin() != distanceLinks.end() && !capture) { // If handlerList is not empty and not captured
		std::vector<uint8_t> inputDataCopy = inputData;
		InputData *parsedInputData = GetMutableInputData(inputDataCopy.data());

		InputMethod *method = distanceLinks.begin()->method;
		InputHandler *handler = distanceLinks.begin()->handler;
		method->updateInput(parsedInputData, handler);

		handler->sendInput(oldFrame, distanceLinks, inputDataCopy);
	}
}

void Grabbable::grab(matrix grabMat) {
	if(xInteract.hasActiveChanged()) {
		zoneable = !xInteract.isActive();
		matrix_inverse(grabMat, startGrabMat);
		startItemMat = transform;
	} else {
		setTransformMatrix(startItemMat * startGrabMat * grabMat);
	}
}

bool Grabbable::handInput(HandInput *hand) {
	const sk::vec3 pinchPos = hand->localToSpacePoint(nullptr, hand->fingerJoints[4].position + hand->fingerJoints[9].position) * 0.5f;
	pinchDistance = field->distance(hand, pinchPos);
	if(pinchDistance > maxDistance && !xInteract.isActive())
		return false;
	float pinchStrength = std::get<float>(hand->datamap["pinchStrength"]);
	xInteract.input(pinchStrength > 0.9f);

	if(xInteract.isActive()) {
		matrix pinchMat = matrix_trs(pinchPos, hand->localToSpaceRotation(nullptr, hand->palmJoint.orientation), vec3_one);
		grab(pinchMat);
	}
	return xInteract.isActive();
}

bool Grabbable::pointerInput(PointerInput *pointer) {
	const sk::vec3 pointerOriginGlobal = pointer->localToSpacePoint(nullptr, vec3_zero);
	const sk::vec3 pointerDirectionGlobal = pointer->localToSpaceDirection(nullptr, vec3_forward);
	const sk::quat pointerRotationGlobal = pointer->localToSpaceRotation(nullptr, quat_identity);

	const float deepestPointDistance = std::get<float>(pointer->datamap["deepestPointDistance"]);
	const sk::vec3 deepestPoint = pointerOriginGlobal + (pointerDirectionGlobal * deepestPointDistance);
	pointDistance = field->distance(nullptr, deepestPoint);
	if(pointDistance > maxDistance && !xInteract.isActive())
		return false;
	const float context = std::get<float>(pointer->datamap["context"]);
	xInteract.input(context > 0.9f);

	if(xInteract.isActive()) {
		matrix pointMat = matrix_trs(pointerOriginGlobal, pointerRotationGlobal, vec3_one);
		grab(pointMat);
		vec2 scroll = std::get<sk::vec2>(pointer->datamap["scroll"]);
		if(scroll.y != 0.0f) {
			position += (pointerDirectionGlobal * scroll.y * 0.00025f);
			transformDirty();
			localTransform();
			startItemMat = transform;
			matrix_inverse(pointMat, startGrabMat);
		}
	}
	return xInteract.isActive();
}

}
