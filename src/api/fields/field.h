#ifndef STARDUSTAPI_FIELD_H
#define STARDUSTAPI_FIELD_H

#include <QVector2D>
#include <QtQuick3D/private/qquick3dnode_p.h>

namespace StardustAPI {
namespace Fields {

class Field : public QQuick3DNode {
    Q_OBJECT
    Q_PROPERTY(float normalRadius MEMBER normalRadius)
public:
    explicit Field(QQuick3DNode *parent = nullptr);

    float normalRadius = 0.0000001f;

    Q_INVOKABLE virtual float localDistance(const QVector3D point) const;
    Q_INVOKABLE virtual QVector3D localClosestPoint(const QVector3D point) const;
    Q_INVOKABLE virtual QVector3D localNormal(const QVector3D point) const;
    Q_INVOKABLE virtual QVector3D localNormal(const QVector3D point, const float radius) const;

    Q_INVOKABLE float distance(const QVector3D point) const;
    Q_INVOKABLE QVector3D closestPoint(const QVector3D point) const;
    Q_INVOKABLE QVector3D normal(const QVector3D point) const;
    Q_INVOKABLE QVector3D normal(const QVector3D point, const float radius) const;
};

}
}

#endif // STARDUSTAPI_FIELD_H
