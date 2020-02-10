QT += qml quick quick3d-private quick3d-openxr widgets
CONFIG += c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH +=	-i/usr/local/include
LIBS += -lopenxr_loader

RESOURCES += src/qml.qrc \
    resources/resources.qrc \
    src/api/api.qrc

HEADERS += \
    src/panel.h \
    src/singletons.h \
    src/core/fileio.h \
    src/core/launcher.h \
    src/core/paths.h \
    src/core/plugininterfaces.h \
    src/core/inputmanager.h \
    src/module/module.h \
    src/module/moduleloader.h \
    src/api/fields/booleanfield.h \
    src/api/fields/boxfield.h \
    src/api/fields/field.h \
    src/api/fields/spherefield.h \
    src/api/fields/unionfield.h \
    src/api/input/inputtypes.h \
    src/api/input/methods/inputdevice.h \
    src/api/input/methods/input.h \
    src/api/input/methods/controllerinput.h \
    src/api/input/methods/globalinput.h \
    src/api/input/methods/handinput.h \
    src/api/input/methods/pointerinput.h \
    src/api/input/methods/toolinput.h \
    src/api/input/components/input0d.h \
    src/api/input/components/input1d.h \
    src/api/input/components/input2d.h \
    src/api/input/handling/actiontrigger.h \
    src/api/input/handling/inputhandler.h \
    src/api/input/keyboard/keyboard.h \
    src/api/input/keyboard/keyboardhandler.h \
    src/api/input/keyboard/passthroughkeyboardhandler.h \
    src/api/input/keyboard/physicalkeyboardadapter.h

SOURCES += \
    src/main.cpp \
    src/core/fileio.cpp \
    src/core/launcher.cpp \
    src/core/paths.cpp \
    src/core/inputmanager.cpp \
    src/module/moduleloader.cpp \
    src/module/module.cpp \
    src/api/input/handling/actiontrigger.cpp \
    src/api/input/handling/inputhandler.cpp \
    src/api/input/methods/controllerinput.cpp \
    src/api/input/methods/globalinput.cpp \
    src/api/input/methods/handinput.cpp \
    src/api/input/methods/pointerinput.cpp \
    src/api/input/keyboard/keyboard.cpp \
    src/api/input/keyboard/keyboardhandler.cpp \
    src/api/input/keyboard/passthroughkeyboardhandler.cpp \
    src/api/input/keyboard/physicalkeyboardadapter.cpp
