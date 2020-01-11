#ifndef PASSTHROUGHKEYBOARDHANDLER_H
#define PASSTHROUGHKEYBOARDHANDLER_H

#include "keyboardhandler.h"
#include <QCoreApplication>

namespace StardustAPI {

class PassthroughKeyboardHandler : public KeyboardHandler
{
    Q_OBJECT
    Q_PROPERTY(QObject *target MEMBER target)
public:
    explicit PassthroughKeyboardHandler(QObject *parent = nullptr);

    QObject *target;

    void qKeyEvent(QKeyEvent *event) override;

//    // KeyboardHandler interface
//public slots:
//    void fullKeyEvent(QObject *keyEvent) override;
//    void keyEvent(int key, bool pressed);
};

}

#endif // PASSTHROUGHKEYBOARDHANDLER_H
