#ifndef PATHS_H
#define PATHS_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>

namespace Stardust {

class Paths : public QObject {
    Q_OBJECT
public:
    explicit Paths(QObject *parent = nullptr);
    Q_INVOKABLE QString getSystemConfigPath();
    Q_INVOKABLE QString getModulesPath();

private:
    QString systemConfigPath = QString("/etc/stardust.json.conf");
    QString modulesPath = QString("/usr/lib/stardust/modules");
    QDir *dir;
    QStandardPaths *standardPaths;

};

}

#endif // PATHS_H
