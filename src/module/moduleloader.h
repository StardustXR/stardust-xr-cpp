#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <QObject>
#include "../core/paths.h"
#include "module.h"

namespace Stardust {

class ModuleLoader : public QObject
{
    Q_OBJECT
public:
    explicit ModuleLoader(Paths *paths = nullptr, QQmlEngine *engine = nullptr);

    Q_INVOKABLE void getModuleList();
    Q_INVOKABLE QObject *loadModule(QString name, bool isStatic);
    Q_INVOKABLE QObject *loadModule(QString name);

    QString modulesStringifiedJSON();

    QQmlEngine *getQmlEngine() const;

private:
    Paths *paths = nullptr;
    QVector<Module> *moduleList = nullptr;

    QQmlEngine *qmlEngine;
};

}


#endif // MODULELOADER_H
