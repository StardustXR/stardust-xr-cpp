#ifndef EXTENSIONLOADER_H
#define EXTENSIONLOADER_H

#include <QList>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlParserStatus>
#include <QQmlComponent>
#include "configpathgetter.h"

class ExtensionLoader : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString configDir READ configDirName WRITE setConfigDirName)
    Q_PROPERTY(QString extensionType READ type WRITE setType)
    Q_PROPERTY(QList<QString> extensionsList READ extensionsList)
public:
    explicit ExtensionLoader(QObject *parent = nullptr);
    Q_INVOKABLE void reload();
    QString configDirName();
    void setConfigDirName(QString &name);
    QString type();
    void setType(QString &type);

    QList<QString> extensionsList();

    void classBegin() override;
    void componentComplete() override;

private:
    QString m_configDirName = "stardust/extensions";
    QString m_extensionType;
    QDir m_extensionsDir;
    QFile m_extensionsFile;
    QList<QString> m_extensionsList;
    void setup();
};

#endif // EXTENSIONLOADER_H
