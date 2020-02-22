#ifndef STARDUSTAPI_INPUT_POINTERHOVERACTIONTRIGGER_H
#define STARDUSTAPI_INPUT_POINTERHOVERACTIONTRIGGER_H

#include "actiontrigger.h"
#include "../methods/pointerinput.h"

namespace StardustAPI {
namespace Input {

class PointerHoverActionTrigger : public ActionTrigger
{
    Q_OBJECT
public:
    PointerHoverActionTrigger();
protected:
    bool isPerformingAction(Input *input, float distance);
};

} // namespace Input
} // namespace StardustAPI

#endif // STARDUSTAPI_INPUT_POINTERHOVERACTIONTRIGGER_H
