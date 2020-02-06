#ifndef STARDUSTAPI_GLOBALINPUT_H
#define STARDUSTAPI_GLOBALINPUT_H

#include "input.h"

namespace StardustAPI {

class GlobalInput : public Input {
    Q_OBJECT
public:
    explicit GlobalInput(QQuick3DNode *parent) : Input(parent, InputType::Hand) {}
};

}

#endif // STARDUSTAPI_HANDINPUT_H
