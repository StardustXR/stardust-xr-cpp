#include "waylandkeyboardhandler.h"

WaylandKeyboardHandler::WaylandKeyboardHandler(QObject *parent) : KeyboardHandler(parent) {
}


QWaylandSurface * WaylandKeyboardHandler::surf() {
    return m_waylandSurface;
}
void WaylandKeyboardHandler::setSurf(QWaylandSurface *surf) {
    m_waylandSurface = surf;
}


void WaylandKeyboardHandler::componentComplete() {
    m_waylandSeat = new QWaylandSeat(m_waylandSurface->compositor(), QWaylandSeat::Keyboard);
    m_waylandSeat->setParent(this);
    m_waylandSeat->setKeyboardFocus(m_waylandSurface);
}


void WaylandKeyboardHandler::keyEvent(int key, bool pressed) {
    m_waylandSeat->sendKeyEvent(key, pressed);
}

void WaylandKeyboardHandler::qKeyEvent(QKeyEvent *keyEvent) {
    m_waylandSeat->sendFullKeyEvent(keyEvent);
}
