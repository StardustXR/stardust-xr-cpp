#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <QObject>
#include <QKeyEvent>
#include <QDebug>
#include <QQmlProperty>

class KeyboardHandler : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardHandler(QObject *parent = nullptr);
    QKeyEvent qmlKeyEventToQKeyEvent(QObject *keyEvent);

public slots:
    virtual void fullKeyEvent(QObject *keyEvent) = 0;
    virtual void keyEvent(int key, bool pressed) = 0;
};

#endif // KEYBOARDHANDLER_H
