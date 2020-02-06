#ifndef STARDUSTAPI_POINTERINPUT_H
#define STARDUSTAPI_POINTERINPUT_H

#include "input.h"

namespace StardustAPI {

class PointerInput : public Input {
    Q_OBJECT
public:
    explicit PointerInput(QQuick3DNode *parent) : Input(parent, InputType::Controller) {}
};

}

#endif // STARDUSTAPI_POINTERINPUT_H
