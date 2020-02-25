#ifndef STARDUSTAPI_TOOLINPUT_H
#define STARDUSTAPI_TOOLINPUT_H

#include "input.h"
#include "../components/input0d.h"
#include "../components/input1d.h"
#include "../components/input2d.h"
#include <QString>

namespace StardustAPI {
namespace Input {

class ToolInput : public Input {
    Q_OBJECT
public:
    explicit ToolInput(QQuick3DNode *parent, InputType inputType) : Input(parent, inputType) {}

    QHash<QString, Input0D> inputs0d;
    QHash<QString, Input1D> inputs1d;
    QHash<QString, Input2D> inputs2d;
};

}
}

#endif // STARDUSTAPI_TOOLINPUT_H
