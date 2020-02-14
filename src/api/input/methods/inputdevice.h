#ifndef STARDUSTAPI_INPUTDEVICE_H
#define STARDUSTAPI_INPUTDEVICE_H

#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QList>
#include <QVariantList>
#include "input.h"

namespace StardustAPI {
namespace Input {

class InputDevice : public QQuick3DNode {
    Q_OBJECT
    Q_PROPERTY(QVariantList inputs READ variantInputs WRITE setInputsVariant)
public:
    explicit InputDevice(QQuick3DNode *parent = nullptr) : QQuick3DNode(parent) {}

    QList<Input *> inputs;
    QVariantList variantInputs();
    void setInputsVariant(QVariantList variantInputs);
};

}
}

#endif // STARDUSTAPI_INPUTDEVICE_H
