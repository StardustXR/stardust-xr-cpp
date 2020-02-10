#ifndef STARDUSTAPI_INPUT_H
#define STARDUSTAPI_INPUT_H

#include <QtQuick3D/private/qquick3dnode_p.h>
#include "../inputtypes.h"
#include "../../fields/field.h"

namespace StardustAPI {
namespace Input {

class InputDevice;

class Input : public QQuick3DNode {
    Q_OBJECT
public:
    explicit Input(QQuick3DNode *parent, InputType inputType) : QQuick3DNode(parent), type(inputType) {}

    bool enabled = true;
    bool rejectAction = false;

    InputType type;
    InputDevice *device;
    virtual float distanceToField(Field *field) = 0;
};

}
}

#endif // STARDUSTAPI_INPUT_H
