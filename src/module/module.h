#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QStringList>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>

#include <QPluginLoader>
#include <QQmlComponent>
#include <QQmlEngine>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Stardust {

class ModuleLoader;

class Module : public QObject {
    Q_OBJECT
public:
    enum State {
        None,
        Success,
        Failure
    };

    explicit Module(ModuleLoader *loader, QString path);

    State reloadModuleInfo();

protected:
    QString name = "";
    QString description = "";
    QString author = "";
    QString sourceURL = "";
    QString websiteURL = "";

    QString id = "";
    QString version = "1.0.0";

    ModuleLoader *moduleLoader;
    QDir *directory = nullptr;

    QVector<Module> dependencies;
    QVector<QPluginLoader> binaries;
    QVector<QQmlComponent> qmlFiles;

    QFile *moduleJsonFile;
    QFile *configJsonFile;

    QJsonDocument moduleJson;
    QJsonDocument configJson;

    QByteArray loadDocument(QFile &file);
    QString getJsonStringKeyValue(QJsonDocument obj, QString key);

};

}

#endif // MODULE_H
