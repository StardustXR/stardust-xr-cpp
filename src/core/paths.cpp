#include "paths.h"

namespace Stardust {

Paths::Paths(QObject *parent) : QObject(parent) {
    dir = new QDir();
}

QString Paths::getSystemConfigPath() {
    return systemConfigPath;
}

QString Paths::getModulesPath() {
    return modulesPath;
}

}
