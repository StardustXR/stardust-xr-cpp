#include "pointerhoveractiontrigger.h"

namespace StardustAPI {
namespace Input {

PointerHoverActionTrigger::PointerHoverActionTrigger() {}


bool PointerHoverActionTrigger::isPerformingAction(Input *input, float distance) {
    Q_UNUSED(input)
    Q_UNUSED(distance)
    return true;
}


} // namespace Input
} // namespace StardustAPI
