#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QKeyEvent>

class Keyboard : public QObject
{
    Q_OBJECT
public:
    explicit Keyboard(QObject *parent = nullptr) : QObject(parent) {}


signals:
    void fullKeyEvent(QObject *keyEvent);
    void keyEvent(int key, bool pressed);

public slots:
};

#endif // KEYBOARD_H
