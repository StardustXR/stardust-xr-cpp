#include "unionfield.h"

namespace StardustAPI {

float UnionField::localDistance(const QVector3D point) const {
    float minimumDistance = std::numeric_limits<float>::max();
    foreach (Field *field, fields) {
        float fieldDistance = field->localDistance(point);
        if(fieldDistance < minimumDistance) {
            minimumDistance = fieldDistance;
        }
    }
    return minimumDistance;
}

}
