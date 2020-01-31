#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <QObject>

#include "../core/paths.h"
#include "../core/fileio.h"
#include "module.h"

namespace Stardust {

class ModuleLoader : public QQuick3DNode
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap moduleInfo READ getModuleInfo)
    Q_PROPERTY(QList<Module *> modules READ getModuleList)

public:
    explicit ModuleLoader(Paths *paths = nullptr, QQmlEngine *engine = nullptr);

    Q_INVOKABLE void reloadModuleList();
    Q_INVOKABLE void loadAllModules();
    Q_INVOKABLE Module *loadModule(QString id);
    Q_INVOKABLE Module *getModuleById(QString id);

    QQmlEngine *qmlEngine;

    QVariantMap getModuleInfo();
    QList<Module *> getModuleList();

private:
    Paths *paths = nullptr;

    QList<Module *> moduleList;
    QDir modulesFolder;

    QJsonObject moduleJSON;
    FileIO systemConfigReader;
};

}


#endif // MODULELOADER_H
