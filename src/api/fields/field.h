#ifndef STARDUSTAPI_FIELD_H
#define STARDUSTAPI_FIELD_H

#include <QVector2D>
#include <QVector3D>
#include <QtQuick3D/private/qquick3dnode_p.h>

namespace StardustAPI {

class Field : public QQuick3DNode {
    Q_OBJECT
public:
    explicit Field(QQuick3DNode *parent);

    virtual float distance(const QVector3D point) const {
        return point.distanceToPoint(closestPoint(point));
    }
    virtual QVector3D closestPoint(const QVector3D point) const = 0;
    virtual QVector3D normal(const QVector3D point) const {
        float d = distance(point);
        QVector2D e(.01f, 0);

        QVector3D n = QVector3D(d,d,d) - QVector3D(
                    distance(QVector3D(e.x(), e.y(), e.y())),
                    distance(QVector3D(e.y(), e.x(), e.y())),
                    distance(QVector3D(e.y(), e.y(), e.x())));

        return n.normalized();
    }
};

}

#endif // STARDUSTAPI_FIELD_H
