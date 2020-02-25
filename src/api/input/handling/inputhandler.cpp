#include "inputhandler.h"

namespace StardustAPI {
namespace Input {

InputHandler::InputHandler(QQuick3DNode *parent) : QQuick3DNode(parent) {}

QQmlListProperty<ActionTrigger> InputHandler::qmlActionTriggers()
{
    return {this, &actionTriggers,
             &InputHandler::appendActionTrigger,
             &InputHandler::actionTriggerCount,
             &InputHandler::actionTrigger,
             &InputHandler::clearActionTriggers};
}

void InputHandler::appendActionTrigger(QQmlListProperty<ActionTrigger> *property, ActionTrigger *p) {
    InputHandler *handler = qobject_cast<InputHandler *>(property->object);
    handler->actionTriggers.append(p);
}

int InputHandler::actionTriggerCount(QQmlListProperty<ActionTrigger> *property) {
    InputHandler *handler = qobject_cast<InputHandler *>(property->object);
    return handler->actionTriggers.count();
}

ActionTrigger *InputHandler::actionTrigger(QQmlListProperty<ActionTrigger> *property, int index) {
    InputHandler *handler = qobject_cast<InputHandler *>(property->object);
    return handler->actionTriggers.at(index);
}

void InputHandler::clearActionTriggers(QQmlListProperty<ActionTrigger> *property) {
    InputHandler *handler = qobject_cast<InputHandler *>(property->object);
    handler->actionTriggers.clear();
}

}
}
