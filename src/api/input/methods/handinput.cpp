#include "handinput.h"

namespace StardustAPI {
namespace Input {

float HandInput::distanceToField(StardustAPI::Fields::Field *field) {
    return field->distance(this->mapPositionToScene(localInteractPoint));
}

}
}
