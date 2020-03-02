#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QStringList>
#include <QVariant>

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

#include <QtQuick3D/private/qquick3dnode_p.h>

namespace Stardust {

class ModuleLoader;

class Module : public QQuick3DNode {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString description MEMBER description)
    Q_PROPERTY(QString author MEMBER author)
    Q_PROPERTY(QString sourceURL MEMBER sourceURL)
    Q_PROPERTY(QString websiteURL MEMBER websiteURL)
    Q_PROPERTY(QString id MEMBER id)
    Q_PROPERTY(QString version MEMBER version)
    Q_PROPERTY(State state MEMBER state)
public:
    enum State {
        None,
        Error,
        Analyzed,
        BinariesLoaded,
        QmlLoaded,
        Instanced
    };
    explicit Module(ModuleLoader *loader = nullptr, QString path = "");

    void reloadModuleInfo();
    Q_INVOKABLE void load();

protected:
    State state = State::None;

    QString name = "";
    QString description = "";
    QString author = "";
    QString sourceURL = "";
    QString websiteURL = "";

    QString id = "";
    QString version = "1.0.0";

    QList<Module *> dependencies;
    QList<QPluginLoader *> binaries;
    QList<QQmlComponent *> qmlComponents;

    QList<QQmlError> errors;

    QList<QQmlComponent *> loadingQmlComponents;

    QQmlComponent *componentFromQmlFileName(QString fileName);

private:
    QFile *moduleJsonFile;
    QJsonObject moduleJson;

    QFile *moduleConfigJsonFile;
    QJsonObject moduleConfigJson;
    QFile *systemConfigJsonFile;
    QJsonObject systemConfigJson;
    QFile *userConfigJsonFile;
    QJsonObject userConfigJson;

    QByteArray loadDocument(QFile &file);
    QString getJsonStringKeyValue(QJsonObject obj, QString key);

    ModuleLoader *moduleLoader;
    QDir *directory = nullptr;


private slots:
    void qmlComponentStatusChanged();
};

}

#endif // MODULE_H
