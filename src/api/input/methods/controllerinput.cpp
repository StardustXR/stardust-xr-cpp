#include "controllerinput.h"

namespace StardustAPI {
namespace Input {

float ControllerInput::distanceToField(StardustAPI::Fields::Field *field) {
    return field->distance(this->mapPositionToScene(localInteractPoint));
}

}
}
