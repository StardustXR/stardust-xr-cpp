#ifndef STARDUSTAPI_INPUT_INPUTHANDLER_H
#define STARDUSTAPI_INPUT_INPUTHANDLER_H

#include <QObject>
#include "actiontrigger.h"
#include "../methods/input.h"
#include "../../fields/field.h"

namespace StardustAPI {
namespace Input {

class InputHandler : public QObject
{
    Q_OBJECT
public:
    explicit InputHandler(QObject *parent = nullptr);

    float maxDistance = 0.01f;
    Field *field = nullptr;

    bool rejectAction = false;

    QList<ActionTrigger *> actionTriggers;

};

}
}

#endif // STARDUSTAPI_INPUT_INPUTHANDLER_H
