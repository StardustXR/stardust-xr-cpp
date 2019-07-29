#include "keyboardhandler.h"

KeyboardHandler::KeyboardHandler(QObject *parent) : QObject(parent) {

}

QKeyEvent *KeyboardHandler::qmlKeyEventToQKeyEvent(QObject *keyEvent) {
    QKeyEvent *event = new QKeyEvent(
                keyEvent->property("pressed").toBool() ? QEvent::KeyPress : QEvent::KeyRelease,
                keyEvent->property("key").toInt(),
                Qt::KeyboardModifiers(keyEvent->property("modifiers").toInt()),
                keyEvent->property("nativeScanCode").toInt(),
                keyEvent->property("nativeScanCode").toInt(),
                keyEvent->property("modifiers").toInt(),
                keyEvent->property("text").toString(),
                keyEvent->property("isAutoRepeat").toBool(),
                keyEvent->property("count").toInt()
            );
//    qDebug() << event->modifiers().testFlag(Qt::ShiftModifier) << QKeySequence(event->key()).toString() << event->text();

    return event;
}

void KeyboardHandler::fullKeyEvent(QObject *keyEvent) {
    emit qKeyEvent(qmlKeyEventToQKeyEvent(keyEvent));
}

void KeyboardHandler::keyEvent(int key, bool pressed) {
    QKeyEvent *keyEvent = new QKeyEvent(pressed ? QEvent::KeyPress : QEvent::KeyRelease, key, Qt::KeyboardModifier::NoModifier);
    emit qKeyEvent(keyEvent);
}
