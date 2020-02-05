#ifndef STARDUSTAPI_SPHEREFIELD_H
#define STARDUSTAPI_SPHEREFIELD_H

#include "field.h"
#include <math.h>

using namespace std;

namespace StardustAPI {

class SphereField : public Field {
    Q_OBJECT
    Q_PROPERTY(QVector3D size MEMBER size)
public:
    explicit SphereField(QQuick3DNode *parent) : Field(parent) {}

    QVector3D size;

    float distance(const QVector3D point) const {
        QVector3D offsetPoint = mapPositionFromScene(point);
        offsetPoint.setX(abs(offsetPoint.x()) - (size.x() / 2));
        offsetPoint.setY(abs(offsetPoint.y()) - (size.y() / 2));
        offsetPoint.setZ(abs(offsetPoint.z()) - (size.z() / 2));

        QVector3D outsideVector = QVector3D(0,0,0);
        outsideVector.setX(max(offsetPoint.x(), 0.0f));
        outsideVector.setY(max(offsetPoint.y(), 0.0f));
        outsideVector.setZ(max(offsetPoint.z(), 0.0f));

        float outsideDistance = outsideVector.length();
        float insideDistance = min(max(offsetPoint.x(), max(offsetPoint.y(), offsetPoint.z())), 0.0f);

        return outsideDistance + insideDistance;
    }
};

}

#endif // STARDUSTAPI_SPHEREFIELD_H
