#include "extensionloader.h"

ExtensionLoader::ExtensionLoader(QObject *parent) : QObject(parent){
    setup();
}

void ExtensionLoader::classBegin() {}
void ExtensionLoader::componentComplete() {
    reload();
}

void ExtensionLoader::setup() {
    ConfigPathGetter getter;
    m_extensionsDir.setPath(getter.loadConfigDir("stardust/extensions"));
}

void ExtensionLoader::reload() {
    setup();
    m_extensionsDir.mkdir(m_extensionType);
    m_extensionsDir.cd(m_extensionType);
    QStringList filterList;
    filterList << "*.qml";
    QFileInfoList extensionsList = m_extensionsDir.entryInfoList(filterList, QDir::Files | QDir::Readable, QDir::Name);


    m_extensionsList = QList<QString>();

    for(int i=0; i<extensionsList.length(); i++) {
        QFileInfo fileInfo = extensionsList[i];
        m_extensionsList.append(fileInfo.filePath());
    }
}

QString ExtensionLoader::configDirName() {
    return m_configDirName;
}
void ExtensionLoader::setConfigDirName(QString &name) {
    m_configDirName = name;
}

QString ExtensionLoader::type() {
    return m_extensionType;
}
void ExtensionLoader::setType(QString &type) {
    m_extensionType = type;
}

QList<QString> ExtensionLoader::extensionsList() {
    return m_extensionsList;
}
