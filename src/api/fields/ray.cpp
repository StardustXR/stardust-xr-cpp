#include "ray.h"
#include "field.h"

namespace StardustAPI {
namespace Fields {

Ray::Ray(QObject *parent) : QObject(parent) {}

RayMarchInfo Ray::march(const Field *field) {
    RayMarchInfo marchInfo;

    marchInfo.closestDistance = maxRayDistance;

    float rayLength = 0.0f;
    QVector3D marchPoint = sceneOrigin;
    QVector3D marchDirection = sceneDirection;
    while(rayLength < maxRayDistance && marchInfo.steps < maxSteps) {
        float distance = field->distance(marchPoint);
        if(distance < marchInfo.closestDistance)
            marchInfo.closestDistance = distance;
        float marchDistance = std::min(std::max(distance, minMarchDistance), maxMarchDistance);
        rayLength += marchDistance;
        marchPoint += marchDistance*marchDirection;
        marchInfo.steps++;
    }
    marchInfo.hitSurface = marchInfo.closestDistance < 0;

    return marchInfo;
}

} // namespace Fields
} // namespace StardustAPI
