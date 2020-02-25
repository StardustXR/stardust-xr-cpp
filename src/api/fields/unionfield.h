#ifndef STARDUSTAPI_UNIONFIELD_H
#define STARDUSTAPI_UNIONFIELD_H

#include "booleanfield.h"

namespace StardustAPI {
namespace Fields {

class UnionField : public BooleanField {
    Q_OBJECT
public:
    explicit UnionField(QQuick3DNode *parent = nullptr) : BooleanField(parent) {}

    float distance(const QVector3D point) const;
};

}
}

#endif // STARDUSTAPI_UNIONFIELD_H
