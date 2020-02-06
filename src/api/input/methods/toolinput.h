#ifndef STARDUSTAPI_CONTROLLERINPUT_H
#define STARDUSTAPI_CONTROLLERINPUT_H

#include "input.h"

namespace StardustAPI {

class ToolInput : public Input {
    Q_OBJECT
public:
    explicit ToolInput(QQuick3DNode *parent, StardustAPI::InputType inputType) : Input(parent, inputType) {}


};

}

#endif // STARDUSTAPI_CONTROLLERINPUT_H
