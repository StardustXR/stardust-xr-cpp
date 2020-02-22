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

    float localDistance(const QVector3D point) const;
    QVector3D localClosestPoint(const QVector3D point) const;
    QVector3D localNormal(const QVector3D point) const;
};

}

#endif // STARDUSTAPI_SPHEREFIELD_H
