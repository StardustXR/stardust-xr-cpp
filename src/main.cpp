#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <QGuiApplication>
#include <QQmlEngine>
#include <Quick3DOpenXR/QOpenXRApplication>

//#include "core/launcher.h"
//#include "core/fileio.h"

//#include "core/configpathgetter.h"

//#include "core/prefs.h"
//#include "core/extensionloader.h"
//#include "core/pluginloader.h"

//#include "keyboard/physicalkeyboardadapter.h"
//#include "keyboard/passthroughkeyboardhandler.h"

QOpenXRApplication *xrApp;

void registerQMLTypes() {
    qmlRegisterSingletonInstance<QOpenXRApplication>("QtQuick3D.OpenXR", 1, 0, "OpenXR", xrApp);

//    qmlRegisterType<Launcher>("Stardust.Core", 1, 0, "Launcher");
//    qmlRegisterType<FileIO>("Stardust.Core", 1, 0, "FileIO");

//    qmlRegisterType<ConfigPathGetter>("Stardust.Core", 1, 0, "ConfigPathGetter");

//    qmlRegisterType<Stardust::Prefs>("Stardust.Core", 1, 0, "Preferences");
//    qmlRegisterType<Stardust::ExtensionLoader>("Stardust.Core", 1, 0, "ExtensionLoader");

//    qmlRegisterSingletonType<Stardust::PluginLoader>("Stardust.Core", 1, 0, "PluginLoader", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
//        Q_UNUSED(engine)
//        Q_UNUSED(scriptEngine)

//        return new Stardust::PluginLoader;
//    });

//    qmlRegisterType<StardustAPI::PhysicalKeyboardAdapter>("Stardust.Core", 1, 0, "PhysicalKeyboardAdapter");
//    qmlRegisterType<StardustAPI::PassthroughKeyboardHandler>("Stardust.Core", 1, 0, "PassthroughKeyboardHandler");
}

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QGuiApplication app(argc, argv);

    xrApp = new QOpenXRApplication(nullptr);

    registerQMLTypes();

    QQmlEngine *mainQmlEngine = new QQmlEngine();
    QQmlComponent *sceneComponent = new QQmlComponent(mainQmlEngine, QUrl("qrc:/core/SceneRoot.qml"), QQmlComponent::PreferSynchronous);

    xrApp->initialize(mainQmlEngine, sceneComponent);

    return app.exec();
}
