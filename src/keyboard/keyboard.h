#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QKeyEvent>

class Keyboard : public QObject
{
    Q_OBJECT
public:
    explicit Keyboard(QObject *parent = nullptr) : QObject(parent) {}
    QObject *qKeyEventToQmlEvent(QKeyEvent *keyEvent);

signals:
    void fullKeyEvent(QObject *keyEvent);
    void keyEvent(int key, bool pressed);
};

#endif // KEYBOARD_H
