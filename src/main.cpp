#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <QGuiApplication>
#include <QQmlEngine>

//#include "core/fileio.h"

//#include "keyboard/physicalkeyboardadapter.h"
//#include "keyboard/passthroughkeyboardhandler.h"

#include "singletons.h"
#include "module/module.h"

#include "api/fields/field.h"
#include "api/fields/spherefield.h"
#include "api/fields/booleanfield.h"
#include "api/fields/unionfield.h"

#include "api/input/methods/input.h"

#include <Quick3DOpenXR/QOpenXRApplication>

using namespace Stardust;

QOpenXRApplication *xrApp;

void registerQMLTypes() {
    qmlRegisterSingletonInstance<QOpenXRApplication>("QtQuick3D.OpenXR", 1, 0, "OpenXR", xrApp);

//    qmlRegisterType<Launcher>("Stardust.Core", 1, 0, "Launcher");
//    qmlRegisterType<FileIO>("Stardust.Core", 1, 0, "FileIO");

//    qmlRegisterType<StardustAPI::PhysicalKeyboardAdapter>("Stardust.Core", 1, 0, "PhysicalKeyboardAdapter");
//    qmlRegisterType<StardustAPI::PassthroughKeyboardHandler>("Stardust.Core", 1, 0, "PassthroughKeyboardHandler");

    qmlRegisterSingletonInstance("StardustAPI.Core", 1, 0, "Paths", Stardust::PathsSingleton());
    qmlRegisterType<Stardust::Module>("Stardust.Core", 1, 0, "Module");
    qmlRegisterSingletonInstance("Stardust.Core", 1, 0, "ModuleLoader", Stardust::ModuleLoaderSingleton());

    qmlRegisterUncreatableType<StardustAPI::Field>("StardustAPI", 1, 0, "Field", "Base class for all fields");
    qmlRegisterType<StardustAPI::SphereField>("StardustAPI", 1, 0, "SphereField");
    qmlRegisterType<StardustAPI::UnionField>("StardustAPI", 1, 0, "UnionField");

//    qmlRegisterType<StardustAPI::Input::Input>("StardustAPI.Input", 1, 0, "Input");
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    xrApp = new QOpenXRApplication(nullptr);

    registerQMLTypes();

    QQmlEngine *mainQmlEngine = new QQmlEngine();
    QQmlComponent *sceneComponent = new QQmlComponent(mainQmlEngine, QUrl("qrc:/core/SceneRoot.qml"), QQmlComponent::PreferSynchronous);

    xrApp->initialize(mainQmlEngine, sceneComponent);

    return app.exec();
}
