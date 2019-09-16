#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>
#include <QPluginLoader>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "../core/configpathgetter.h"

#include <QQmlEngine>

namespace Stardust {

class PluginLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString JSON READ pluginJSON)
public:
    explicit PluginLoader(QObject *parent = nullptr);
    ~PluginLoader();

    static PluginLoader *pluginLoader;

    Q_INVOKABLE void getPluginsList();
    Q_INVOKABLE QObject *loadPlugin(QString name, bool isStatic);
    Q_INVOKABLE QObject *loadPlugin(QString name);

    QString pluginJSON();

    void registerSingleton(QQmlEngine *qmlEngine);

private:
    ConfigPathGetter *m_configPathGetter = nullptr;
    QJsonArray *m_pluginList = nullptr;
};

}

#endif // PLUGINLOADER_H
