#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include <openxr/openxr.h>

#include <QFile>
#include <QDir>

#include <QtQuick3D/private/qquick3dviewport_p.h>

#include "core/launcher.h"
#include "core/fileio.h"

#include "core/paths.h"
#include "module/moduleloader.h"

#include "keyboard/physicalkeyboardadapter.h"
#include "keyboard/passthroughkeyboardhandler.h"

#include "openxr/stardustvulkan.h"
#include "openxr/stardustopenxr.h"
#include "openxr/stardustopenxrgraphics.h"

Stardust::OpenXR *openxr;
Stardust::Vulkan *vulkan;
Stardust::OpenXRGraphics *graphics;

QQmlEngine *mainQmlEngine;
QQuickItem *rootObject;

Stardust::Paths *paths;
Stardust::ModuleLoader *moduleLoader;

void loadQML() {
    mainQmlEngine = new QQmlEngine();

    //Create QML component
    QQmlComponent *rootComponent = new QQmlComponent(mainQmlEngine, "qrc:/core/StereoRender.qml", QQmlComponent::PreferSynchronous);

    //Load in the QML and add it to the window
    rootObject = qobject_cast<QQuickItem *>(graphics->qmlComponent->create());
    graphics->rootObject = rootObject;
}

void registerQMLTypes() {
    qmlRegisterType<Launcher>("Stardust.Core", 1, 0, "Launcher");
    qmlRegisterType<FileIO>("Stardust.Core", 1, 0, "FileIO");

    qmlRegisterType<StardustAPI::PhysicalKeyboardAdapter>("Stardust.Core", 1, 0, "PhysicalKeyboardAdapter");
    qmlRegisterType<StardustAPI::PassthroughKeyboardHandler>("Stardust.Core", 1, 0, "PassthroughKeyboardHandler");

    qmlRegisterSingletonType<Stardust::Vulkan>("Stardust.Core.Internal", 1, 0, "Vulkan", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return vulkan;
    });
    qmlRegisterSingletonType<Stardust::OpenXR>("Stardust.Core.Internal", 1, 0, "OpenXR", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return openxr;
    });
    qmlRegisterSingletonType<Stardust::OpenXRGraphics>("Stardust.Core.Internal", 1, 0, "OpenXRGraphics", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return graphics;
    });

    qmlRegisterSingletonType<Stardust::Paths>("Stardust.Core", 1, 0, "Paths", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return paths;
    });

    //Modules
    qmlRegisterType<Stardust::Module>("Stardust.Core", 1, 0, "Module");
    qmlRegisterSingletonType<Stardust::ModuleLoader>("Stardust.Core", 1, 0, "ModuleLoader", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return moduleLoader;
    });
}

void initOpenXR() {
    graphics = new Stardust::OpenXRGraphics();
    graphics->qmlEngine = mainQmlEngine;
    graphics->preInitialize();

    openxr = new Stardust::OpenXR();
    vulkan = new Stardust::Vulkan();

    openxr->vulkan = vulkan;
    vulkan->openxr = openxr;

    openxr->initialize();

    graphics->openxr = openxr;

    graphics->initialize();
}

int main(int argc, char *argv[]) {
    // ShareOpenGLContexts is needed for using the threaded renderer
    // on Nvidia EGLStreams
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QGuiApplication app(argc, argv);

    loadQML();
    registerQMLTypes();

    paths = new Stardust::Paths();
    moduleLoader = new Stardust::ModuleLoader(paths, mainQmlEngine);

    initOpenXR();

    QSurfaceFormat::setDefaultFormat(QQuick3DViewport::idealSurfaceFormat());

    return app.exec();
}
