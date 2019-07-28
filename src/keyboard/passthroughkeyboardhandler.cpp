#include "passthroughkeyboardhandler.h"

PassthroughKeyboardHandler::PassthroughKeyboardHandler(QObject *parent) : KeyboardHandler (parent) {}

void PassthroughKeyboardHandler::fullKeyEvent(QObject *event) {
    QKeyEvent *keyEvent = qmlKeyEventToQKeyEvent(event);
    QCoreApplication::postEvent(target, keyEvent);
}

void PassthroughKeyboardHandler::keyEvent(int key, bool pressed) {
    QCoreApplication::postEvent(target,
        new QKeyEvent(pressed ? QEvent::KeyPress : QEvent::KeyRelease, key, Qt::NoModifier));
}
