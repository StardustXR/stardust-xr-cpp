QT += qml quick quick3d-private quick3d-openxr widgets
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/core/fileio.cpp \
    src/core/launcher.cpp \
    src/core/paths.cpp \
    src/module/moduleloader.cpp \
    src/module/module.cpp \
#    src/pointer/inputpointer.cpp \
    src/keyboard/keyboard.cpp \
    src/keyboard/keyboardhandler.cpp \
    src/keyboard/passthroughkeyboardhandler.cpp \
    src/main.cpp \
    src/keyboard/physicalkeyboardadapter.cpp

RESOURCES += src/qml.qrc \
    defaults/defaults.qrc \
    resources/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=	-i/usr/local/include

LIBS += -lopenxr_loader

HEADERS += \
    src/singletons.h \
    src/core/fileio.h \
    src/core/launcher.h \
    src/module/module.h \
    src/module/moduleloader.h \
    src/core/paths.h \
    src/core/plugininterfaces.h \
#    src/pointer/inputpointer.h \
    src/keyboard/keyboard.h \
    src/keyboard/keyboardhandler.h \
    src/keyboard/passthroughkeyboardhandler.h \
    src/keyboard/physicalkeyboardadapter.h
