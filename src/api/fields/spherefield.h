#ifndef STARDUSTAPI_SPHEREFIELD_H
#define STARDUSTAPI_SPHEREFIELD_H

#include "field.h"

namespace StardustAPI {

class SphereField : public Field {
    Q_OBJECT
    Q_PROPERTY(float radius MEMBER radius)
public:
    explicit SphereField(QQuick3DNode *parent = nullptr) : Field(parent) {}

    float radius = 1.0f;

    float distance(const QVector3D point) const {
        QVector3D localPoint = this->mapPositionFromScene(point);
        return localPoint.length() - radius;
    }
    QVector3D localClosestPoint(const QVector3D point) const {
        QVector3D localPoint = mapPositionFromScene(point);
        QVector3D dir = localPoint.normalized();
        QVector3D localClosePoint = dir * radius;
        return localClosePoint;
    }
    QVector3D closestPoint(const QVector3D point) const {
        return mapPositionToScene(localClosestPoint(point));
    }
    QVector3D normal(const QVector3D point) const {
        return mapDirectionToScene(localClosestPoint(point)).normalized();
    }
};

}

#endif // STARDUSTAPI_SPHEREFIELD_H
