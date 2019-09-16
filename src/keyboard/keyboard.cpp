#include "keyboard.h"

namespace StardustAPI {

QObject *Keyboard::qKeyEventToQmlEvent(QKeyEvent *keyEvent) {
    QObject *event = new QObject(this);

    event->setProperty("pressed", keyEvent->type() == QEvent::KeyPress);
    event->setProperty("key", keyEvent->key());
    event->setProperty("modifiers", static_cast<uint>(keyEvent->modifiers()));
    event->setProperty("nativeScanCode", keyEvent->nativeScanCode());
    event->setProperty("text", keyEvent->text());
    event->setProperty("isAutoRepeat", keyEvent->isAutoRepeat());
    event->setProperty("count", keyEvent->count());

    return event;
}

}
