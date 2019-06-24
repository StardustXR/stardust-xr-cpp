#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

class Keyboard : public QObject
{
    Q_OBJECT
public:
    explicit Keyboard(QObject *parent = nullptr);

signals:

public slots:
};

#endif // KEYBOARD_H
