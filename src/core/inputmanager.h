#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QObject>
#include <QList>

namespace Stardust {

class InputManager : public QObject {
    Q_OBJECT
public:
    explicit InputManager(QObject *parent = nullptr);

public slots:
    void processInputs();

private:
//    QList<
};

}

#endif // INPUTMANAGER_H
