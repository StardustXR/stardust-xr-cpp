#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <QObject>

#include "../core/paths.h"
#include "../core/fileio.h"
#include "module.h"

namespace Stardust {

class ModuleLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap modules READ getModuleList)
public:
    explicit ModuleLoader(Paths *paths = nullptr, QQmlEngine *engine = nullptr);

    Q_INVOKABLE void reloadModuleList();
    Q_INVOKABLE void loadAllModules();
    Q_INVOKABLE void loadModule(QString id);
    Q_INVOKABLE Module *getModuleById(QString id);

    QQmlEngine *qmlEngine;

    QVariantMap getModuleList();

private:
    Paths *paths = nullptr;

    QVector<Module *> moduleList;
    QDir modulesFolder;

    QJsonObject moduleJSON;
    FileIO systemConfigReader;
};

}


#endif // MODULELOADER_H
