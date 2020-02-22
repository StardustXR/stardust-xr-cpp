#ifndef STARDUSTAPI_POINTERINPUT_H
#define STARDUSTAPI_POINTERINPUT_H

#include "toolinput.h"
#include <math.h>

namespace StardustAPI {
namespace Input {

class PointerInput : public ToolInput {
    Q_OBJECT
    Q_PROPERTY(QVector3D pointerOrigin MEMBER localPointerOrigin)
    Q_PROPERTY(QVector3D pointerDirection MEMBER localPointerDirection)
public:
    explicit PointerInput(QQuick3DNode *parent = nullptr) : ToolInput(parent, InputType::Controller) {}

    QVector3D localPointerOrigin;
    QVector3D localPointerDirection;

    float minMarchDistance = 0.001f;
    float maxRayLength = 100.0f;
    uint maxRaySteps = 1000;

    virtual float distanceToField(StardustAPI::Fields::Field *field);
};

}
}

#endif // STARDUSTAPI_POINTERINPUT_H
