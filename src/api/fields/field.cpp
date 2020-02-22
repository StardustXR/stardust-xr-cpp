#include "field.h"

namespace StardustAPI {
namespace Fields {

Field::Field(QQuick3DNode *parent) : QQuick3DNode(parent) {}

float Field::localDistance(const QVector3D point) const {
    return point.distanceToPoint(localClosestPoint(point));
}

QVector3D Field::localClosestPoint(const QVector3D point) const {
    return point - (localNormal(point) * localDistance(point));
}

QVector3D Field::localNormal(const QVector3D point) const {
    return localNormal(point, normalRadius);
}

QVector3D Field::localNormal(const QVector3D point, const float radius) const {
    float d = localDistance(point);
        QVector2D e(radius, 0.0f);

        QVector3D n = QVector3D(d,d,d) - QVector3D(
                    localDistance(QVector3D(e.x(), e.y(), e.y())),
                    localDistance(QVector3D(e.y(), e.x(), e.y())),
                    localDistance(QVector3D(e.y(), e.y(), e.x())));

        return n.normalized();
}

float Field::distance(const QVector3D point) const {
    QVector3D localPoint = mapPositionFromScene(point);
    if(sceneScale() == QVector3D(1.0f, 1.0f, 1.0f)) {
        //If the field is not scaled
        return localDistance(localPoint);
    } else if(fabsf(sceneScale().x()-sceneScale().y()) <= 0.00001f && fabsf(sceneScale().y()-sceneScale().z()) <= 0.00001f) {
        //If the field is scaled uniformly
        float scaleFactor = sceneScale().x();
        return localDistance(localPoint*scaleFactor);
    } else {
        //If the field is scaled non-uniformly. Should be avoided if possible due to inefficiency.
        QVector3D localSurfacePoint = localClosestPoint(localPoint);
        float localScaledDistance = localDistance(localPoint); //Get the distance so we know if the point is inside or outside
        QVector3D surfacePoint = mapPositionToScene(localSurfacePoint);
        float distance = point.distanceToPoint(surfacePoint);
        return copysignf(distance, localScaledDistance); //Apply the sign of the DE to tell if the point is inside/outside
    }
}

QVector3D Field::closestPoint(const QVector3D point) const {
    QVector3D localPoint = mapPositionFromScene(point);
    return mapPositionToScene(localClosestPoint(localPoint));
}

QVector3D Field::normal(const QVector3D point) const {
    return normal(point, normalRadius);
}

QVector3D Field::normal(const QVector3D point, const float radius) const {
    QVector3D localPoint = mapPositionFromScene(point);
    return mapDirectionToScene(localNormal(localPoint, radius));
}

}
}
