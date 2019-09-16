#ifndef STARDUSTOPENGL_H
#define STARDUSTOPENGL_H

#include <QObject>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <qpa/qplatformnativeinterface.h>
#include <QScreen>

#include "openxr_meta.h"

namespace Stardust {

class OpenGL : public QObject
{
    Q_OBJECT
public:
    explicit OpenGL(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();

    QPlatformNativeInterface *nativeInterface;
    QQuickWindow *window;

//    GLXContext context;
//    GLXDrawable drawable;
};

}


#endif // STARDUSTOPENGL_H
