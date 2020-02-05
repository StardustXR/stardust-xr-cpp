#ifndef STARDUSTAPI_FIELD_H
#define STARDUSTAPI_FIELD_H

#include <QtQuick3D/private/qquick3dnode_p.h>

namespace StardustAPI {

class Field : public QQuick3DNode {
    Q_OBJECT
public:
    explicit Field(QQuick3DNode *parent);

    virtual float distance(const QVector3D point) const = 0;
    virtual QVector3D closestPoint(const QVector3D point) const = 0;
    virtual QVector3D normal(const QVector3D point) const = 0;
};

}

#endif // STARDUSTAPI_FIELD_H
