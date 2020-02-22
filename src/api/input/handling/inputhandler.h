#ifndef STARDUSTAPI_INPUT_INPUTHANDLER_H
#define STARDUSTAPI_INPUT_INPUTHANDLER_H

#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QQmlListProperty>
#include "actiontrigger.h"
#include "../methods/input.h"
#include "../../fields/field.h"

namespace StardustAPI {
namespace Input {

class InputHandler : public QQuick3DNode
{
    Q_OBJECT
    Q_PROPERTY(float maxDistance MEMBER maxDistance)
    Q_PROPERTY(Field *field MEMBER field)
    Q_PROPERTY(QQmlListProperty<StardustAPI::Input::ActionTrigger> actionTriggers READ qmlActionTriggers)
    Q_CLASSINFO("DefaultProperty", "actionTriggers")
public:
    explicit InputHandler(QQuick3DNode *parent = nullptr);

    float maxDistance = 0.01f;
    Field *field = nullptr;

    bool rejectAction = false;

    QList<ActionTrigger *> actionTriggers;
    QQmlListProperty<ActionTrigger> qmlActionTriggers();

private:
    static void appendActionTrigger(QQmlListProperty<ActionTrigger>*, ActionTrigger *p);
    static int actionTriggerCount(QQmlListProperty<ActionTrigger>*);
    static ActionTrigger *actionTrigger(QQmlListProperty<ActionTrigger>*, int index);
    static void clearActionTriggers(QQmlListProperty<ActionTrigger>*);
};

}
}

#endif // STARDUSTAPI_INPUT_INPUTHANDLER_H
