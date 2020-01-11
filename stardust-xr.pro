QT += qml quick quick3d-private quick3d-openxr widgets
CONFIG += c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH +=	-i/usr/local/include
LIBS += -lopenxr_loader

RESOURCES += src/qml.qrc \
    defaults/defaults.qrc \
    resources/resources.qrc

HEADERS += \
    src/singletons.h \
    src/core/fileio.h \
    src/core/launcher.h \
    src/core/paths.h \
    src/core/plugininterfaces.h \
    src/module/module.h \
    src/module/moduleloader.h \
#    src/api/input/pointer/inputpointer.h \
    src/api/input/keyboard/keyboard.h \
    src/api/input/keyboard/keyboardhandler.h \
    src/api/input/keyboard/passthroughkeyboardhandler.h \
    src/api/input/keyboard/physicalkeyboardadapter.h

SOURCES += \
    src/main.cpp \
    src/core/fileio.cpp \
    src/core/launcher.cpp \
    src/core/paths.cpp \
    src/module/moduleloader.cpp \
    src/module/module.cpp \
#    src/api/input/pointer/inputpointer.cpp \
    src/api/input/keyboard/keyboard.cpp \
    src/api/input/keyboard/keyboardhandler.cpp \
    src/api/input/keyboard/passthroughkeyboardhandler.cpp \
    src/api/input/keyboard/physicalkeyboardadapter.cpp
