#ifndef STARDUSTAPI_HANDINPUT_H
#define STARDUSTAPI_HANDINPUT_H

#include "input.h"

namespace StardustAPI {

class HandInput : public Input {
    Q_OBJECT
public:
    explicit HandInput(QQuick3DNode *parent) : Input(parent, InputType::Hand) {}
};

}

#endif // STARDUSTAPI_HANDINPUT_H
