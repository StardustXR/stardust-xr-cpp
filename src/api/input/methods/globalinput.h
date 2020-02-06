#ifndef STARDUSTAPI_GLOBALINPUT_H
#define STARDUSTAPI_GLOBALINPUT_H

#include "toolinput.h"

namespace StardustAPI {

class GlobalInput : public ToolInput {
    Q_OBJECT
public:
    explicit GlobalInput(QQuick3DNode *parent) : ToolInput(parent, InputType::Global) {}
};

}

#endif // STARDUSTAPI_HANDINPUT_H
