#ifndef STARDUSTAPI_POINTERINPUT_H
#define STARDUSTAPI_POINTERINPUT_H

#include "toolinput.h"

namespace StardustAPI {

class PointerInput : public ToolInput {
    Q_OBJECT
public:
    explicit PointerInput(QQuick3DNode *parent) : ToolInput(parent, InputType::Controller) {}
};

}

#endif // STARDUSTAPI_POINTERINPUT_H
