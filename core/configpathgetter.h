#ifndef CONFIGPATHGETTER_H
#define CONFIGPATHGETTER_H

#include <QObject>
#include <QDir>
#include <QStandardPaths>

class ConfigPathGetter : public QObject
{
    Q_OBJECT
public:
    explicit ConfigPathGetter(QObject *parent = nullptr);
    QDir loadConfigDir(QString name);
    QString loadConfigDirPath(QString name);

private:
    QString m_configPath;
    QDir m_configDir;
};

#endif // CONFIGPATHGETTER_H
