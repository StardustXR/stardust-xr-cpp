#ifndef RECOVERYKEYBOARDADAPTER_H
#define RECOVERYKEYBOARDADAPTER_H

#include <QObject>
#include "keyboard.h"
#include <libinput.h>

using namespace StardustAPI;

namespace Stardust {

class RecoveryKeyboardAdapter : public Keyboard
{
    Q_OBJECT
public:
    explicit RecoveryKeyboardAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();

signals:
    void fullKeyEvent(QObject *keyEvent);
    void keyEvent(int key, bool pressed);
};

}

#endif // RECOVERYKEYBOARDADAPTER_H
