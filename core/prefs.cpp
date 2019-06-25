#include "prefs.h"

Prefs::Prefs(QObject *parent) : QObject(parent)
{
    Q_INIT_RESOURCE(defaults);
    ConfigPathGetter getter;
    m_prefsDir = getter.loadConfigDir("stardust");
}

void Prefs::componentComplete() {
    load();
}

void Prefs::load() {
    m_prefsFile.setFileName(m_prefsDir.absoluteFilePath(m_prefsFileName));
    if(!m_prefsFile.exists()) {
        QFile::copy(":/defaults/"+m_prefsFileName, m_prefsFile.fileName());
    }
    if(m_prefsFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&m_prefsFile);
        QString val = in.readAll();
        m_jsonDocument = QJsonDocument::fromJson(val.toUtf8());
        m_jsonObject = m_jsonDocument.object();
    } else {
        m_jsonObject = QJsonObject();
    }
    m_prefsFile.close();
}

void Prefs::save() {
    m_jsonDocument.setObject(m_jsonObject);
    m_prefsFile.setFileName(m_prefsDir.absoluteFilePath(m_prefsFileName));
    if(m_prefsFile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&m_prefsFile);
        out << QString::fromUtf8(m_jsonDocument.toJson());
    }
    m_prefsFile.close();
}

QString Prefs::prefsFileName() {
    return m_prefsFileName;
}
void Prefs::setPrefsFileName(QString &name) {
    m_prefsFileName = name;
}
QString Prefs::configDirName() {
    return m_configDirName;
}
void Prefs::setConfigDirName(QString &name) {
    m_configDirName = name;
}
QJsonObject Prefs::jsonPrefs() {
    return m_jsonObject;
}
void Prefs::setJsonPrefs(QJsonObject &object) {
    m_jsonObject = object;
}
