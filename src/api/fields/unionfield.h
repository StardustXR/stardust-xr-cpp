#ifndef STARDUSTAPI_UNIONFIELD_H
#define STARDUSTAPI_UNIONFIELD_H

#include <limits>
#include "field.h"

namespace StardustAPI {

class UnionField : public Field {
    Q_OBJECT
public:
    explicit UnionField(QQuick3DNode *parent) : Field(parent) {
        connect(this, &QQuick3DObject::childrenChanged, this, &UnionField::updateFieldList);
    }

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

private:
    QList<Field *> fields = QList<Field *>();

private slots:
    void updateFieldList() {
        fields.clear();
        foreach(QQuick3DObject *child, childItems()) {
            Field *field = qobject_cast<Field *>(child);
            if(field != nullptr) {
                fields.append(field);
            }
        }
    }
};

}

#endif // STARDUSTAPI_UNIONFIELD_H
