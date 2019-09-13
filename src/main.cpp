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

#include "core/configpathgetter.h"

#include "core/prefs.h"
#include "core/extensionloader.h"
#include "core/pluginloader.h"

#include "keyboard/physicalkeyboardadapter.h"
#include "keyboard/passthroughkeyboardhandler.h"

#include "openxr/stardustvulkan.h"
#include "openxr/stardustopenxr.h"
#include "openxr/stardustopenxrgraphics.h"

StardustOpenXR *openxr;
StardustVulkan *vulkan;
StardustOpenXRGraphics *graphics;

void registerQMLTypes() {
    qmlRegisterType<Launcher>("Launcher", 1, 0, "Launcher");
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");

    qmlRegisterType<ConfigPathGetter>("ConfigPathGetter", 1, 0, "ConfigPathGetter");

    qmlRegisterType<Prefs>("Preferences", 1, 0, "Preferences");
    qmlRegisterType<ExtensionLoader>("ExtensionLoader", 1, 0, "ExtensionLoader");

    qmlRegisterSingletonType<PluginLoader>("PluginLoader", 1, 0, "PluginLoader", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return new PluginLoader;
    });

    qmlRegisterType<PhysicalKeyboardAdapter>("PhysicalKeyboardAdapter", 1, 0, "PhysicalKeyboardAdapter");
    qmlRegisterType<PassthroughKeyboardHandler>("PassthroughKeyboardHandler", 1, 0, "PassthroughKeyboardHandler");

    qmlRegisterSingletonType<StardustVulkan>("Vulkan", 1, 0, "Vulkan", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return vulkan;
    });
    qmlRegisterSingletonType<StardustOpenXR>("OpenXR", 1, 0, "OpenXR", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return openxr;
    });
    qmlRegisterSingletonType<StardustOpenXRGraphics>("OpenXRGraphics", 1, 0, "OpenXRGraphics", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return graphics;
    });
}

void initOpenXR() {
    graphics = new StardustOpenXRGraphics();
    graphics->preInitialize();

    openxr = new StardustOpenXR();
    vulkan = new StardustVulkan();

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

    registerQMLTypes();

    initOpenXR();

    QSurfaceFormat::setDefaultFormat(QQuick3DViewport::idealSurfaceFormat());

    return app.exec();
}
