#include "inputdevice.h"

namespace StardustAPI {
namespace Input {

InputDevice::InputDevice(QQuick3DNode *parent) : QQuick3DNode(parent) {}

QQmlListProperty<Input> InputDevice::qmlInputs()
{
    return {this, &inputs,
             &InputDevice::appendInput,
             &InputDevice::inputCount,
             &InputDevice::input,
             &InputDevice::clearInputs};
}

void InputDevice::appendInput(QQmlListProperty<Input> *property, Input *p) {
    InputDevice *handler = qobject_cast<InputDevice *>(property->object);
    handler->inputs.append(p);
}

int InputDevice::inputCount(QQmlListProperty<Input> *property) {
    InputDevice *handler = qobject_cast<InputDevice *>(property->object);
    return handler->inputs.count();
}

Input *InputDevice::input(QQmlListProperty<Input> *property, int index) {
    InputDevice *handler = qobject_cast<InputDevice *>(property->object);
    return handler->inputs.at(index);
}

void InputDevice::clearInputs(QQmlListProperty<Input> *property) {
    InputDevice *handler = qobject_cast<InputDevice *>(property->object);
    handler->inputs.clear();
}

}
}
