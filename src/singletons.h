//Create singletons for objects
#define SINGLETON_GENERATOR(CLASS)                   extern CLASS *_##CLASS; CLASS *_##CLASS = nullptr; CLASS *CLASS##Singleton(){if(_##CLASS == nullptr){_##CLASS = new CLASS();} return _##CLASS;}
#define SINGLETON_GENERATOR_1ARG(CLASS, ARG1)        extern CLASS *_##CLASS; CLASS *_##CLASS = nullptr; CLASS *CLASS##Singleton(){if(_##CLASS == nullptr){_##CLASS = new CLASS(ARG1);} return _##CLASS;}
#define SINGLETON_GENERATOR_2ARGS(CLASS, ARG1, ARG2) extern CLASS *_##CLASS; CLASS *_##CLASS = nullptr; CLASS *CLASS##Singleton(){if(_##CLASS == nullptr){_##CLASS = new CLASS(ARG1, ARG2);} return _##CLASS;}
/*
SINGLETON_GENERATOR(QObject)

becomes

QObject *_QObject = nullptr;
//#include "core/paths.h"
QObject *QObjectSingleton() {
    if(_QObject == nullptr)
        _QObject = new Paths();
    return _QObject;
}
Paths
and can be accessed by Stardust::QObjectSingleton();
*/

#include <QQmlEngine>

#include "core/launcher.h"
#include "core/paths.h"
#include "module/moduleloader.h"

namespace Stardust {
    SINGLETON_GENERATOR(QQmlEngine)

    SINGLETON_GENERATOR(Launcher)
    SINGLETON_GENERATOR(Paths)
    SINGLETON_GENERATOR_2ARGS(ModuleLoader, PathsSingleton(), QQmlEngineSingleton())
}
