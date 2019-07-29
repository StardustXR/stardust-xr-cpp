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
    virtual void qKeyEvent(QKeyEvent *keyEvent) = 0;
    QKeyEvent *qmlKeyEventToQKeyEvent(QObject *keyEvent);

public slots:
    virtual void fullKeyEvent(QObject *keyEvent);
    virtual void keyEvent(int key, bool pressed);
};

#endif // KEYBOARDHANDLER_H
