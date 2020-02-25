#ifndef STARDUSTAPI_POINTERINPUT_H
#define STARDUSTAPI_POINTERINPUT_H

#include "toolinput.h"
#include <math.h>
#include "../../fields/ray.h"

using namespace StardustAPI::Fields;

namespace StardustAPI {
namespace Input {

class PointerInput : public ToolInput {
    Q_OBJECT
    Q_PROPERTY(Ray *pointerRay MEMBER ray)
public:
    explicit PointerInput(QQuick3DNode *parent = nullptr) : ToolInput(parent, InputType::Pointer) {}

    Ray *ray = nullptr;

    virtual float distanceToField(StardustAPI::Fields::Field *field);
};

}
}

#endif // STARDUSTAPI_POINTERINPUT_H
