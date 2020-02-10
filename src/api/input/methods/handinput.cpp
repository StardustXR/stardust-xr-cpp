#include "handinput.h"

namespace StardustAPI {
namespace Input {

float HandInput::distanceToField(Field *field) {
    return field->distance(this->mapPositionToScene(localInteractPoint));
}

}
}
