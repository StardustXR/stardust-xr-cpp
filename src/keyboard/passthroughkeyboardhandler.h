#ifndef PASSTHROUGHKEYBOARDHANDLER_H
#define PASSTHROUGHKEYBOARDHANDLER_H

#include "keyboardhandler.h"
#include <QCoreApplication>

class PassthroughKeyboardHandler : public KeyboardHandler
{
    Q_OBJECT
    Q_PROPERTY(QObject *target MEMBER target)
public:
    explicit PassthroughKeyboardHandler(QObject *parent = nullptr);

    QObject *target;

public slots:
    void fullKeyEvent(QObject *event) override;
    void keyEvent(int key, bool pressed) override;
};

#endif // PASSTHROUGHKEYBOARDHANDLER_H
