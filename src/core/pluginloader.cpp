#include "pluginloader.h"

#include "../core/plugininterfaces.h"
#include <QDebug>

PluginLoader::PluginLoader(QObject *parent) : QObject(parent) {
    m_configPathGetter = new ConfigPathGetter(this);
    getPluginsList();
}

PluginLoader::~PluginLoader() {
    delete m_configPathGetter;
    delete m_pluginList;
}

void PluginLoader::getPluginsList() {
    if(m_pluginList)
        delete m_pluginList;
    m_pluginList = new QJsonArray();

    qDebug() << "Looking for plugins..." << endl;

    const QVector<QStaticPlugin> staticPlugins = QPluginLoader::staticPlugins();
    for (int i=0; i<staticPlugins.length(); i++) {
        QJsonObject jsonObj = staticPlugins[i].metaData();
        jsonObj["static"] = true;
        m_pluginList->append(jsonObj);
        qDebug() << "Found static keyboard plugin " << jsonObj["MetaData"].toObject()["name"].toString() << "with metadata"  << endl << jsonObj << endl;
        qDebug() << jsonObj << endl;
    }

    QDir pluginsDir(m_configPathGetter->loadConfigDir("stardust/extensions/keyboard"));

    const auto entryList = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entryList) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            QJsonObject jsonObj = loader.metaData();
            jsonObj["static"] = false;
            jsonObj["filePath"] = pluginsDir.absoluteFilePath(fileName);
            m_pluginList->append(jsonObj);
            qDebug() << "Found dynamic keyboard plugin" << jsonObj["MetaData"].toObject()["name"].toString() << "with metadata"  << endl << jsonObj << endl;
        } else {
            qDebug() << loader.errorString() << endl;
        }
    }
}

QObject *PluginLoader::loadPlugin(QString name, bool isStatic) {
    if(isStatic) {
        for(int i=0; i<QPluginLoader::staticPlugins().length(); i++) {
            QStaticPlugin *plugin = &QPluginLoader::staticPlugins()[i];
            if(plugin->metaData()["MetaData"].toObject()["name"].toString() == name) {
                qDebug() << "Loaded static keyboard plugin" << plugin->metaData()["MetaData"].toObject()["name"].toString() << endl;

                return plugin->instance();
            }
        }
    } else {
        for(int i=0; i<m_pluginList->count(); i++) {
            if(m_pluginList->at(i).toObject()["MetaData"].toObject()["name"].toString() == name) {
                qDebug() << "Loaded dynamic keyboard plugin" << m_pluginList->at(i).toObject()["MetaData"].toObject()["name"].toString() << endl;

                QPluginLoader plugin(m_pluginList->at(i).toObject()["filePath"].toString());
                return plugin.instance();
            }
        }
    }
}

QObject *PluginLoader::loadPlugin(QString name) {
    for(int i=0; i<m_pluginList->count(); i++) {
        QJsonObject metadataObject = m_pluginList->at(i).toObject()["MetaData"].toObject();
        if(metadataObject["name"].toString() == name) {
            return loadPlugin(name, metadataObject["static"].toBool());
        }
    }
}

QString PluginLoader::pluginJSON() {
    QJsonDocument doc(*m_pluginList);
    return QString::fromUtf8(doc.toJson());
}
