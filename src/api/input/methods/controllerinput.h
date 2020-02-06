#ifndef STARDUSTAPI_CONTROLLERINPUT_H
#define STARDUSTAPI_CONTROLLERINPUT_H

#include "toolinput.h"

namespace StardustAPI {
namespace Input {

class ControllerInput : public ToolInput {
    Q_OBJECT
public:
    explicit ControllerInput(QQuick3DNode *parent) : ToolInput(parent, InputType::Controller) {}
};

}
}

#endif // STARDUSTAPI_CONTROLLERINPUT_H
