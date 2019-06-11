#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <QtGui/QGuiApplication>

#include <QtQml/QQmlApplicationEngine>

#include "launcher.h"
#include "fileio.h"

int main(int argc, char *argv[])
{
    // ShareOpenGLContexts is needed for using the threaded renderer
    // on Nvidia EGLStreams
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QGuiApplication app(argc, argv);

    qmlRegisterType<Launcher>("Launcher", 1, 0, "Launcher");
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");

    QQmlApplicationEngine appEngine(QUrl("qrc:///main.qml"));

    return app.exec();
}
