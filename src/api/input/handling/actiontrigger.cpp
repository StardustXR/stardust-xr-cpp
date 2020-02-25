#include "actiontrigger.h"

namespace StardustAPI {
namespace Input {

ActionTrigger::ActionTrigger(QObject *parent, InputType type) : QObject(parent) {
    this->type = type;
}

void ActionTrigger::testAction(Input *input, float distance) {
    performedAction = performingAction;

    //Check if the input types are the same
    bool inputTypeMatches = input->type == type;
    if(inputTypeMatches)
        performingAction = isPerformingAction(input, distance);
    else
        performingAction = false;

    //Emit events when there is a change in action being performed or not
    if(performedAction != performingAction) {
        if(performingAction)
            emit actionStarted(input, distance);
        if(performedAction)
            emit actionEnded(input, distance);
    }

    if(performingAction) {
        emit action(input, distance);
        actioningInput = input;
    } else {
        actioningInput = nullptr;
    }
}

} // namespace Input
} // namespace StardustAPI
