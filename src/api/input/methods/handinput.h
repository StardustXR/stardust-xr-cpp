#ifndef STARDUSTAPI_HANDINPUT_H
#define STARDUSTAPI_HANDINPUT_H

#include "input.h"

namespace StardustAPI {
namespace Input {

class HandInput : public Input {
    Q_OBJECT
public:
    explicit HandInput(QQuick3DNode *parent = nullptr) : Input(parent, InputType::Hand) {}

    QVector3D localInteractPoint;
    float distanceToField(StardustAPI::Fields::Field *field);
};

}
}

#endif // STARDUSTAPI_HANDINPUT_H
