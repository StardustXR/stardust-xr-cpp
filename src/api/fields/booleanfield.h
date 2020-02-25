#ifndef STARDUSTAPI_BOOLEANFIELD_H
#define STARDUSTAPI_BOOLEANFIELD_H

#include "field.h"

namespace StardustAPI {
namespace Fields {

class BooleanField : public Field {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<StardustAPI::Fields::Field> fields READ qmlFields)
    Q_CLASSINFO("DefaultProperty", "fields")
public:
    explicit BooleanField(QQuick3DNode *parent = nullptr);

protected:
    QList<Field *> fields = QList<Field *>();
    QQmlListProperty<Field> qmlFields();

private:
    static void appendField(QQmlListProperty<Field>*, Field *p);
    static int fieldsCount(QQmlListProperty<Field>*);
    static Field *field(QQmlListProperty<Field>*, int index);
    static void clearFields(QQmlListProperty<Field>*);

protected slots:
    void updateFieldList();
};

}
}

#endif // STARDUSTAPI_BOOLEANFIELD_H
