#ifndef STARDUSTAPI_INPUT_H
#define STARDUSTAPI_INPUT_H

#include <QtQuick3D/private/qquick3dnode_p.h>

namespace StardustAPI {

enum InputType {
    Hand,
    Controller,
    Pointer,
    Global
};

class InputDevice;

class Input : public QQuick3DNode {
    Q_OBJECT
public:
    explicit Input(QQuick3DNode *parent, InputType inputType) : QQuick3DNode(parent), type(inputType) {}

    InputType type;
    InputDevice *device;
};

}

#endif // STARDUSTAPI_INPUT_H
