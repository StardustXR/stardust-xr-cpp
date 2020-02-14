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

#include "api/input/handling/actiontrigger.h"
#include "api/input/handling/inputhandler.h"
#include "api/input/methods/inputdevice.h"
#include "api/input/methods/input.h"
#include "api/input/methods/handinput.h"
#include "api/input/methods/controllerinput.h"
#include "api/input/methods/pointerinput.h"
#include "api/input/methods/globalinput.h"

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

    qmlRegisterUncreatableType<StardustAPI::Field>("StardustAPI.Fields", 1, 0, "Field", "Base class for all fields");
    qmlRegisterType<StardustAPI::SphereField>("StardustAPI.Fields", 1, 0, "SphereField");
    qmlRegisterType<StardustAPI::UnionField>("StardustAPI.Fields", 1, 0, "UnionField");

    qmlRegisterUncreatableType<StardustAPI::Input::ActionTrigger>("StardustAPI.Input", 1, 0, "ActionTrigger", "Base class for all action triggers");
    qmlRegisterType<StardustAPI::Input::InputHandler>("StardustAPI.Input", 1, 0, "InputHandler");
    qmlRegisterUncreatableType<StardustAPI::Input::InputHandler>("StardustAPI.Input", 1, 0, "InputHandler", "Global input handler");
    qmlRegisterSingletonInstance("StardustAPI.Input", 1, 0, "InputManager", Stardust::InputManagerSingleton());
    qmlRegisterType<StardustAPI::Input::InputDevice>("StardustAPI.Input", 1, 0, "InputDevice");
    qmlRegisterUncreatableType<StardustAPI::Input::Input>("StardustAPI.Input", 1, 0, "Input", "Base class for all inputs");
    qmlRegisterType<StardustAPI::Input::HandInput>("StardustAPI.Input", 1, 0, "HandInput");
    qmlRegisterType<StardustAPI::Input::ControllerInput>("StardustAPI.Input", 1, 0, "ControllerInput");
    qmlRegisterType<StardustAPI::Input::PointerInput>("StardustAPI.Input", 1, 0, "PointerInput");
    qmlRegisterType<StardustAPI::Input::GlobalInput>("StardustAPI.Input", 1, 0, "GlobalInput");
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    xrApp = new QOpenXRApplication(nullptr);

    registerQMLTypes();
    QObject::connect(xrApp, &QOpenXRApplication::frame, Stardust::InputManagerSingleton(), &Stardust::InputManager::processInputs);

    QQmlEngine *mainQmlEngine = new QQmlEngine();
    QQmlComponent *sceneComponent = new QQmlComponent(mainQmlEngine, QUrl("qrc:/core/SceneRoot.qml"), QQmlComponent::PreferSynchronous);

    xrApp->initialize(mainQmlEngine, sceneComponent);

    return app.exec();
}
