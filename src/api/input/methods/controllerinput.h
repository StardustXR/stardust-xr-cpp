#ifndef STARDUSTAPI_CONTROLLERINPUT_H
#define STARDUSTAPI_CONTROLLERINPUT_H

#include "input.h"

namespace StardustAPI {

class ControllerInput : public Input {
    Q_OBJECT
public:
    explicit ControllerInput(QQuick3DNode *parent) : Input(parent, InputType::Controller) {}
};

}

#endif // STARDUSTAPI_CONTROLLERINPUT_H
