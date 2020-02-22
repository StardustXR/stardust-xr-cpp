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
    Q_PROPERTY(QQmlListProperty<StardustAPI::Input::Input> inputs READ qmlInputs)
    Q_CLASSINFO("DefaultProperty", "inputs")
public:
    explicit InputDevice(QQuick3DNode *parent = nullptr);

    QList<Input *> inputs;
    QQmlListProperty<Input> qmlInputs();

private:
    static void appendInput(QQmlListProperty<Input>*, Input *p);
    static int inputCount(QQmlListProperty<Input>*);
    static Input *input(QQmlListProperty<Input>*, int index);
    static void clearInputs(QQmlListProperty<Input>*);
};

}
}

#endif // STARDUSTAPI_INPUTDEVICE_H
