#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <QObject>
#include <QPluginLoader>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "configpathgetter.h"

namespace Stardust {

class ModuleLoader : public QObject
{
    Q_OBJECT
public:
    explicit ModuleLoader(QObject *parent = nullptr);

    Q_INVOKABLE void getModuleList();
    Q_INVOKABLE QObject *loadModule(QString name, bool isStatic);
    Q_INVOKABLE QObject *loadModule(QString name);

    QString modulesStringifiedJSON();

private:
    ConfigPathGetter *configPathGetter = nullptr;
    QJsonArray *moduleList = nullptr;
};

}


#endif // MODULELOADER_H
