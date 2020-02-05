#ifndef STARDUSTAPI_UNIONFIELD_H
#define STARDUSTAPI_UNIONFIELD_H

#include "booleanfield.h"

namespace StardustAPI {

class UnionField : public BooleanField {
    Q_OBJECT
public:
    explicit UnionField(QQuick3DNode *parent) : BooleanField(parent) {}

    float distance(const QVector3D point) const {
        float minimumDistance = std::numeric_limits<float>::max();
        foreach (Field *field, fields) {
            float fieldDistance = field->distance(point);
            if(fieldDistance < minimumDistance) {
                minimumDistance = fieldDistance;
            }
        }
        return minimumDistance;
    }
};

}

#endif // STARDUSTAPI_UNIONFIELD_H
