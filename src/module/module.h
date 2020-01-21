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

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString description MEMBER description)
    Q_PROPERTY(QString author MEMBER author)
    Q_PROPERTY(QString sourceURL MEMBER sourceURL)
    Q_PROPERTY(QString websiteURL MEMBER websiteURL)
    Q_PROPERTY(QString id MEMBER id)
    Q_PROPERTY(QString version MEMBER version)
public:
    enum State {
        None,
        Success,
        Failure
    };
    explicit Module(ModuleLoader *loader = nullptr, QString path = "");

    State reloadModuleInfo();
    void loadModule(QObject autostartParent);

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

    QList<Module *> dependencies;
    QList<QPluginLoader *> binaries;
    QList<QQmlComponent *> qmlComponents;

    QList<QQmlComponent *> loadingQmlComponents;

    QFile *moduleJsonFile;
    QFile *configJsonFile;

    QJsonObject moduleJson;
    QJsonObject configJson;

    QByteArray loadDocument(QFile &file);
    QString getJsonStringKeyValue(QJsonObject obj, QString key);

    QQmlComponent *componentFromQmlFileName(QString fileName);

private slots:
    void qmlComponentStatusChanged();
};

}

#endif // MODULE_H
