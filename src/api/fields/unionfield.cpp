#include "unionfield.h"

namespace StardustAPI {
namespace Fields {

float UnionField::distance(const QVector3D point) const {
    float minimumDistance = std::numeric_limits<float>::max();
    foreach (Field *field, fields) {
        float fieldDistance = field->distance(point);
        if(fieldDistance < minimumDistance) {
            minimumDistance = fieldDistance;
        }
    }
    return minimumDistance;
}

}
}
