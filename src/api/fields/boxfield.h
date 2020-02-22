#ifndef STARDUSTAPI_BOXFIELD_H
#define STARDUSTAPI_BOXFIELD_H

#include "field.h"
#include <math.h>

using namespace std;

namespace StardustAPI {

class BoxField : public Field {
    Q_OBJECT
public:
    explicit BoxField(QQuick3DNode *parent = nullptr) : Field(parent) {}

    QVector3D size;

    float localDistance(const QVector3D point) const;
};

}

#endif // STARDUSTAPI_BOXFIELD_H
