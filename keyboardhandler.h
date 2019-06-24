#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <QObject>

class KeyboardHandler : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardHandler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // KEYBOARDHANDLER_H
