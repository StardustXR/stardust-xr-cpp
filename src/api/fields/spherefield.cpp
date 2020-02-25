#include "spherefield.h"

namespace StardustAPI {
namespace Fields {

float SphereField::localDistance(const QVector3D point) const {
    return point.length() - radius;
}

QVector3D SphereField::localClosestPoint(const QVector3D point) const {
    QVector3D dir = point.normalized();
    QVector3D closePoint = dir * radius;
    return closePoint;
}

QVector3D SphereField::localNormal(const QVector3D point) const {
    return point.normalized();
}

}
}
