#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <QtGui/QGuiApplication>

#include <QtQml/QQmlApplicationEngine>

#include <QFile>
#include <QDir>

#include "core/launcher.h"
#include "core/fileio.h"

#include "core/configpathgetter.h"

#include "core/prefs.h"
#include "core/extensionloader.h"
#include "core/pluginloader.h"

#include "keyboard/physicalkeyboardadapter.h"
#include "keyboard/passthroughkeyboardhandler.h"
#include "keyboard/waylandkeyboardhandler.h"

int main(int argc, char *argv[])
{
    // ShareOpenGLContexts is needed for using the threaded renderer
    // on Nvidia EGLStreams
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QGuiApplication app(argc, argv);


    qmlRegisterType<Launcher>("Launcher", 1, 0, "Launcher");
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");

    qmlRegisterType<ConfigPathGetter>("ConfigPathGetter", 1, 0, "ConfigPathGetter");

    qmlRegisterType<Prefs>("Preferences", 1, 0, "Preferences");
    qmlRegisterType<ExtensionLoader>("ExtensionLoader", 1, 0, "ExtensionLoader");

    qmlRegisterSingletonType<PluginLoader>("PluginLoader", 1, 0, "PluginLoader", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new PluginLoader;
    });

    qmlRegisterType<PhysicalKeyboardAdapter>("PhysicalKeyboardAdapter", 1, 0, "PhysicalKeyboardAdapter");
    qmlRegisterType<PassthroughKeyboardHandler>("PassthroughKeyboardHandler", 1, 0, "PassthroughKeyboardHandler");
    qmlRegisterType<WaylandKeyboardHandler>("WaylandKeyboardHandler", 1, 0, "WaylandKeyboardHandler");

    ConfigPathGetter getter;
    QDir renderSettingsDir = getter.loadConfigDir("stardust");
    if(!QFile::exists(renderSettingsDir.absoluteFilePath("StardustRenderSettings.qml"))) {
        QFile::copy(":/StardustRenderSettings.qml", renderSettingsDir.absoluteFilePath("StardustRenderSettings.qml"));
    }

    QQmlApplicationEngine appEngine(QUrl("qrc:/core/main.qml"));

    return app.exec();
}
