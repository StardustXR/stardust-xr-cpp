#include "configpathgetter.h"

ConfigPathGetter::ConfigPathGetter(QObject *parent) : QObject(parent) {
    m_configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    m_configDir = QDir(m_configPath);
}

QDir ConfigPathGetter::loadConfigDir(QString name) {
    m_configDir.mkpath(m_configPath);
    m_configDir.setPath(m_configPath);
    m_configDir.mkdir(name);
    m_configDir.cd(name);
    return m_configDir;
}

QString ConfigPathGetter::loadConfigDirPath(QString name) {
    m_configDir.mkpath(m_configPath);
    m_configDir.setPath(m_configPath);
    m_configDir.mkdir(name);
    m_configDir.cd(name);
    return m_configDir.path();
}

