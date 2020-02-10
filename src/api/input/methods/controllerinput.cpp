#include "controllerinput.h"

namespace StardustAPI {
namespace Input {

float ControllerInput::distanceToField(Field *field) {
    return field->distance(this->mapPositionToScene(localInteractPoint));
}

}
}
