#include "booleanfield.h"

namespace StardustAPI {
namespace Fields {

BooleanField::BooleanField(QQuick3DNode *parent) : Field(parent) {}

void BooleanField::updateFieldList() {
    fields.clear();
    foreach(QQuick3DObject *child, childItems()) {
        Field *field = qobject_cast<Field *>(child);
        if(field != nullptr) {
            fields.append(field);
        }
    }
}

QQmlListProperty<Field> BooleanField::qmlFields()
{
    return {this, &fields,
             &BooleanField::appendField,
             &BooleanField::fieldsCount,
             &BooleanField::field,
             &BooleanField::clearFields};
}

void BooleanField::appendField(QQmlListProperty<Field> *property, Field *p) {
    BooleanField *boolField = qobject_cast<BooleanField *>(property->object);
    boolField->fields.append(p);
}

int BooleanField::fieldsCount(QQmlListProperty<Field> *property) {
    BooleanField *boolField = qobject_cast<BooleanField *>(property->object);
    return boolField->fields.count();
}

Field *BooleanField::field(QQmlListProperty<Field> *property, int index) {
    BooleanField *boolField = qobject_cast<BooleanField *>(property->object);
    return boolField->fields.at(index);
}

void BooleanField::clearFields(QQmlListProperty<Field> *property) {
    BooleanField *boolField = qobject_cast<BooleanField *>(property->object);
    boolField->fields.clear();
}

}
}
