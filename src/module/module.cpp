#include "module.h"
#include <QDebug>
#include "moduleloader.h"
#include <QQmlEngine>
#include <QQmlContext>

namespace Stardust {

Module::Module(ModuleLoader *loader, QString path) : QObject(nullptr) {
    moduleLoader = loader;
    directory = new QDir(path);

    moduleJsonFile = new QFile(directory->entryInfoList(QStringList("module.json"), QDir::Readable | QDir::Files).first().absoluteFilePath());
    configJsonFile = new QFile(directory->entryInfoList(QStringList("config.json"), QDir::Readable | QDir::Files).first().absoluteFilePath());

    dependencies = QVector<Module *>();
    binaries = QVector<QPluginLoader *>();
    qmlComponents = QVector<QQmlComponent *>();

    reloadModuleInfo();
}

Module::State Module::reloadModuleInfo() {
    moduleJson = QJsonDocument::fromJson(loadDocument(*moduleJsonFile));
    configJson = QJsonDocument::fromJson(loadDocument(*configJsonFile));

    if(moduleJson.isNull() || configJson.isNull())
        return Module::State::Failure;

    name = getJsonStringKeyValue(moduleJson, "name");
    description = getJsonStringKeyValue(moduleJson, "description");
    author = getJsonStringKeyValue(moduleJson, "author");
    sourceURL = getJsonStringKeyValue(moduleJson, "sourceURL");
    websiteURL = getJsonStringKeyValue(moduleJson, "websiteURL");

    id = getJsonStringKeyValue(moduleJson, "id");
    version = getJsonStringKeyValue(moduleJson, "version");

//    if(moduleJson.object().contains("deps")) {
//        QJsonArray dependencyPaths = moduleJson.object()["deps"].toArray();
//    }

//    if(moduleJson.object().contains("binaries")) {
//        QJsonArray binaryPaths = moduleJson.object()["binaries"].toArray();

//        foreach(QVariant binary, binaryPaths.toVariantList()) {
//            QString binaryPath = directory->absoluteFilePath(binary.toString());

//            binaries.push_back(new QPluginLoader());
//        }
//    }

    if(moduleJson.object().contains("qml")) {
        QJsonArray qmlFilePaths = moduleJson.object()["qml"].toArray();

        foreach(QVariant qmlFile, qmlFilePaths.toVariantList()) {
            QString qmlFilePath = directory->absoluteFilePath(qmlFile.toString());

            qmlComponents.push_back(new QQmlComponent(moduleLoader->qmlEngine, qmlFilePath, QQmlComponent::PreferSynchronous));
        }
    }

    return Module::State::Success;
}

QByteArray Module::loadDocument(QFile &file) {
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Failed to open "<<file.fileName();

        return NULL;
    }

    QTextStream file_text(&file);
    QString json_string;
    json_string = file_text.readAll();
    file.close();
    return json_string.toLocal8Bit();
}

QString Module::getJsonStringKeyValue(QJsonDocument obj, QString key) {
    if(!obj.object().contains(key))
        return "";

    return obj.object()[key].toString();
}

}
