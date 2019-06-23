#ifndef PREFS_H
#define PREFS_H

#include <QObject>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QQmlParserStatus>
#include "configpathgetter.h"

class Prefs : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString fileName READ prefsFileName WRITE setPrefsFileName)
    Q_PROPERTY(QString configDirName READ configDirName WRITE setConfigDirName)
    Q_PROPERTY(QJsonObject json READ jsonPrefs WRITE setJsonPrefs NOTIFY jsonChanged)
public:
    explicit Prefs(QObject *parent = nullptr);

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();

    QString prefsFileName();
    void setPrefsFileName(QString &name);
    QString configDirName();
    void setConfigDirName(QString &name);
    QJsonObject jsonPrefs();
    void setJsonPrefs(QJsonObject &object);

    void classBegin() override;
    void componentComplete() override;
private:
    QString m_prefsFileName;
    QString m_configDirName = "stardust";
    QDir m_prefsDir;
    QFile m_prefsFile;
    QJsonDocument m_jsonDocument;
    QJsonObject m_jsonObject;
signals:
    void jsonChanged(QJsonObject newJson);
public slots:
};

#endif // PREFS_H
