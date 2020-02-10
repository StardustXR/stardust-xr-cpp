#ifndef STARDUSTAPI_INPUTDEVICE_H
#define STARDUSTAPI_INPUTDEVICE_H

#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QList>
#include "input.h"

namespace StardustAPI {
namespace Input {

class InputDevice : public QQuick3DNode {
    Q_OBJECT
public:
    explicit InputDevice(QQuick3DNode *parent) : QQuick3DNode(parent) {}

    QList<Input *> inputs;
};

}
}

#endif // STARDUSTAPI_INPUTDEVICE_H
