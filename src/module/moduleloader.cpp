#include "moduleloader.h"
#include <QDebug>

namespace Stardust {

ModuleLoader::ModuleLoader(Paths *paths, QQmlEngine *engine) : QObject(nullptr) {
    this->paths = paths;
    this->qmlEngine = engine;
}


//void ModuleLoader::getModuleList() {
//    if(moduleList)
//        delete moduleList;
//    moduleList = new QJsonArray();

//    qDebug() << "Looking for plugins..." << endl;

//    QDir pluginsDir(paths->getModulesPath());

//    const auto entryList = pluginsDir.entryList(QDir::Files);
//    for (const QString &fileName : entryList) {
//        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
//        QObject *plugin = loader.instance();
//        if (plugin) {
//            QJsonObject jsonObj = loader.metaData();
//            jsonObj["static"] = false;
//            jsonObj["filePath"] = pluginsDir.absoluteFilePath(fileName);
//            moduleList->append(jsonObj);
//        } else {
//            qDebug() << loader.errorString() << endl;
//        }
//    }
//}

//QObject *ModuleLoader::loadModule(QString name, bool isStatic) {
//    if(isStatic) {
//        for(int i=0; i<QPluginLoader::staticPlugins().length(); i++) {
//            QStaticPlugin *plugin = &QPluginLoader::staticPlugins()[i];
//            if(plugin->metaData()["MetaData"].toObject()["name"].toString() == name) {
//                qDebug() << "Loaded static keyboard plugin" << plugin->metaData()["MetaData"].toObject()["name"].toString() << endl;

//                return plugin->instance();
//            }
//        }
//    } else {
//        for(int i=0; i<moduleList->count(); i++) {
//            if(moduleList->at(i).toObject()["MetaData"].toObject()["name"].toString() == name) {
//                qDebug() << "Loaded dynamic keyboard plugin" << moduleList->at(i).toObject()["MetaData"].toObject()["name"].toString() << endl;

//                QPluginLoader plugin(moduleList->at(i).toObject()["filePath"].toString());
//                return plugin.instance();
//            }
//        }
//    }
//}

//QObject *ModuleLoader::loadModule(QString name) {
//    for(int i=0; i<moduleList->count(); i++) {
//        QJsonObject metadataObject = moduleList->at(i).toObject()["MetaData"].toObject();
//        if(metadataObject["name"].toString() == name) {
//            return loadModule(name, metadataObject["static"].toBool());
//        }
//    }
//}

//QString ModuleLoader::modulesStringifiedJSON() {
//    QJsonDocument doc(*moduleList);
//    return QString::fromUtf8(doc.toJson());
//}
}
