#ifndef PHYSICALKEYBOARDADAPTER_H
#define PHYSICALKEYBOARDADAPTER_H

#include <QObject>
#include "keyboard.h"

using namespace StardustAPI;

namespace Stardust {

class PhysicalKeyboardAdapter : public Keyboard
{
    Q_OBJECT
public:
    explicit PhysicalKeyboardAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();

signals:
    void fullKeyEvent(QObject *keyEvent);
    void keyEvent(int key, bool pressed);
};

}

#endif // PHYSICALKEYBOARDADAPTER_H
