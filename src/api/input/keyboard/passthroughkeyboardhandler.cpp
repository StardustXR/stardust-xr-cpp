#include "passthroughkeyboardhandler.h"

namespace StardustAPI {

PassthroughKeyboardHandler::PassthroughKeyboardHandler(QObject *parent) : KeyboardHandler (parent) {}

void PassthroughKeyboardHandler::qKeyEvent(QKeyEvent *event) {
    QCoreApplication::postEvent(target, event);
}

}
