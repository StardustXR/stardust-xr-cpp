#ifndef STARDUSTAPI_BOOLEANFIELD_H
#define STARDUSTAPI_BOOLEANFIELD_H

#include <limits>
#include "field.h"

namespace StardustAPI {

class BooleanField : public Field {
    Q_OBJECT
public:
    explicit BooleanField(QQuick3DNode *parent) : Field(parent) {}

protected:
    QList<Field *> fields = QList<Field *>();

protected slots:
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

#endif // STARDUSTAPI_BOOLEANFIELD_H
