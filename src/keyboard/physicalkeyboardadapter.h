#ifndef PHYSICALKEYBOARDADAPTER_H
#define PHYSICALKEYBOARDADAPTER_H

#include <QObject>
#include "keyboard.h"

class PhysicalKeyboardAdapter : public Keyboard
{
    Q_OBJECT
public:
    explicit PhysicalKeyboardAdapter(QObject *parent = nullptr);

signals:
    void fullKeyEvent(QObject *keyEvent);
    void keyEvent(int key, bool pressed);

public slots:
    void sendKey(QObject *keyEvent, bool pressed);
};

#endif // PHYSICALKEYBOARDADAPTER_H
