#ifndef PHYSICALKEYBOARDADAPTER_H
#define PHYSICALKEYBOARDADAPTER_H

#include <QObject>
#include "keyboard.h"

namespace StardustAPI {

class PhysicalKeyboardAdapter : public Keyboard
{
    Q_OBJECT
public:
    explicit PhysicalKeyboardAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void sendKey(QObject *keyEvent, bool pressed);

signals:
    void fullKeyEvent(QObject *keyEvent);
    void keyEvent(int key, bool pressed);
};

}

#endif // PHYSICALKEYBOARDADAPTER_H
