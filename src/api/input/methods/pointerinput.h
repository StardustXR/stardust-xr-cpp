#ifndef STARDUSTAPI_POINTERINPUT_H
#define STARDUSTAPI_POINTERINPUT_H

#include "toolinput.h"
#include <math.h>

namespace StardustAPI {
namespace Input {

class PointerInput : public ToolInput {
    Q_OBJECT
public:
    explicit PointerInput(QQuick3DNode *parent) : ToolInput(parent, InputType::Controller) {}

    QVector3D localPointerOrigin;
    QVector3D localPointerDirection;

    float minMarchDistance = 0.001f;
    float maxRayLength = 100.0f;
    uint maxRaySteps = 1000;

    virtual float distanceToField(Field *field);
};

}
}

#endif // STARDUSTAPI_POINTERINPUT_H
