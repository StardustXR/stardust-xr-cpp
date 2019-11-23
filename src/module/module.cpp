#include "module.h"
#include <QDebug>

namespace Stardust {

Module::Module(QString path) : QObject(nullptr) {
    directory = new QDir(path);

    moduleJsonFile = new QFile(directory->entryInfoList(QStringList("module.json"), QDir::Readable | QDir::Files).first().absoluteFilePath());
    configJsonFile = new QFile(directory->entryInfoList(QStringList("config.json"), QDir::Readable | QDir::Files).first().absoluteFilePath());

    dependencies = QVector<Module>();
    binaries = QVector<QPluginLoader>();
    qmlFiles = QVector<QQmlComponent>();

    reloadModule();
}

Module::State Module::reloadModule() {
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

    QJsonArray dependencyPaths =
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
