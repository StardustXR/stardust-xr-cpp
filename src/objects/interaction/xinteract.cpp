#include "xinteract.hpp"
#include <cstdio>

namespace StardustXRServer {

XInteract::XInteract() {}

void XInteract::update() {
	interactable = inputActivity != oldInputActivity;
	interactableChanged = interactable != oldInteractable;
	
	oldInteractable = interactable;
	oldInputActivity = inputActivity;
}

void XInteract::input(bool active) {
	inputActivity = !inputActivity;

	if(!interactable && !oldActive)
		activeFirstFrame = active;
	else
		this->active = active;
	if(activeFirstFrame && !active)
		activeFirstFrame = false;
	activeChanged = this->active != oldActive;
	oldActive = active;
}

bool XInteract::isActive() {
	return active && !activeFirstFrame;
}
bool XInteract::hasActiveChanged() {
	return activeChanged;
}

bool XInteract::isInteractable() {
	return interactable;
}
bool XInteract::hasInteractableChanged() {
	return interactableChanged;
}

}
