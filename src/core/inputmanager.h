#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QObject>
#include <QList>
#include "../api/input/methods/inputdevice.h"
#include "../api/input/handling/inputhandler.h"

using namespace StardustAPI::Input;

namespace Stardust {

class InputManager : public QObject {
    Q_OBJECT
public:
    explicit InputManager(QObject *parent = nullptr);

    Q_INVOKABLE void registerInputDevice(InputDevice *device);
    Q_INVOKABLE void registerInputHandler(InputHandler *handler);

public slots:
    void processInputs();

private:
    typedef struct inputHandlerLink {
        Input *input;
        float distance;
        InputHandler *handler;
    } InputHandlerLink;

    QList<InputDevice *> inputDevices;
    QList<Input *> inputs;
    QList<InputHandler *> inputHandlers;
};

}

#endif // INPUTMANAGER_H
