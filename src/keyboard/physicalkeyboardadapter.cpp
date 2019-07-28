#include "physicalkeyboardadapter.h"

PhysicalKeyboardAdapter::PhysicalKeyboardAdapter(QObject *parent) : Keyboard(parent) {}

void PhysicalKeyboardAdapter::sendKey(QObject *keyEvent, bool pressed) {
    keyEvent->setProperty("pressed", pressed);
    emit fullKeyEvent(keyEvent);
}
