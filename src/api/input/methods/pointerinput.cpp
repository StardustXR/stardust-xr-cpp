#include "pointerinput.h"

namespace StardustAPI {
namespace Input {

float PointerInput::distanceToField(StardustAPI::Fields::Field *field) {
    return ray->march(field).closestDistance;
}

}
}
