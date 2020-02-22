#include "pointerinput.h"

namespace StardustAPI {
namespace Input {

float PointerInput::distanceToField(Field *field) {
    uint steps = 0;
    float minDistance = maxRayLength;
    float rayLength = 0.0f;
    QVector3D marchPoint = this->mapPositionToScene(localPointerOrigin);
    QVector3D marchDirection = this->mapDirectionToScene(localPointerDirection);
    while(rayLength < maxRayLength && steps < maxRaySteps) {
        float distance = field->distance(marchPoint);
        if(distance < minDistance)
            minDistance = distance;
        float marchDistance = std::max(distance, minMarchDistance);
        rayLength += marchDistance;
        marchPoint += marchDistance*marchDirection;
        steps++;
    }

    return minDistance;
}

}
}
