#ifndef STARDUSTAPI_INPUT_ACTIONTRIGGER_H
#define STARDUSTAPI_INPUT_ACTIONTRIGGER_H

#include <QObject>
#include "../inputtypes.h"
#include "../methods/input.h"

namespace StardustAPI {
namespace Input {

class ActionTrigger : public QObject
{
    Q_OBJECT
public:
    explicit ActionTrigger(QObject *parent = nullptr);

    InputType type;

    Input *actioningInput = nullptr;

    void testAction(Input *input, float distance);

signals:
    void actionStarted(Input *input, float distance);
    void action(Input *input, float distance);
    void actionEnded(Input *input, float distance);

protected:
    virtual bool isPerformingAction(Input *input, float distance) = 0;

private:
    bool performedAction = false;
    bool performingAction = false;
};

} // namespace Input
} // namespace StardustAPI

#endif // STARDUSTAPI_INPUT_ACTIONTRIGGER_H
