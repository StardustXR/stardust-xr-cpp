#ifndef STARDUSTAPI_FIELDS_RAY_H
#define STARDUSTAPI_FIELDS_RAY_H

#include <QObject>
#include <QVector3D>
#include <cfloat>

namespace StardustAPI {
namespace Fields {

typedef struct {
    bool hitSurface = false;
    uint steps = 0;
    float closestDistance;
} RayMarchInfo;

class Field;

class Ray : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D sceneOrigin MEMBER sceneOrigin)
    Q_PROPERTY(QVector3D sceneDirection MEMBER sceneDirection)
public:
    explicit Ray(QObject *parent = nullptr);

    QVector3D sceneOrigin;
    QVector3D sceneDirection;

    uint maxSteps = 1000;
    float minMarchDistance = 0.001f;
    float maxMarchDistance = FLT_MAX;
    float maxRayDistance = FLT_MAX;

    Q_INVOKABLE RayMarchInfo march(const Field *field);
};

} // namespace Fields
} // namespace StardustAPI

#endif // STARDUSTAPI_FIELDS_RAY_H
