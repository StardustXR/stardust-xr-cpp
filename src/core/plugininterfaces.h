#ifndef PLUGININTERFACES_H
#define PLUGININTERFACES_H

#include <QObject>

namespace StardustAPI {

#define VirtualObjectInterface_iid "org.technobaboo.Stardust.VirtualObjectInterface/1.0"

class VirtualObjectInterface {
public:
    virtual ~VirtualObjectInterface() {}
    Q_INVOKABLE virtual QObject *createInstance(QObject *parent) = 0;
};

}

Q_DECLARE_INTERFACE(StardustAPI::VirtualObjectInterface, VirtualObjectInterface_iid)

#endif // PLUGININTERFACES_H
