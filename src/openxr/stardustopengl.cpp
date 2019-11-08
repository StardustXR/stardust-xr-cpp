#include "stardustopengl.h"

#include <QCoreApplication>
#include <QOpenGLContext>
#include <QtPlatformHeaders/QGLXNativeContext>
#include <string.h>

namespace Stardust {

OpenGL::OpenGL(QObject *parent) : QObject(parent) {

}
void OpenGL::initialize() {
    //Temporarily make the context current
    QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity, true);
    window->openglContext()->doneCurrent();
    window->openglContext()->makeCurrent(window);


    //Get X display
    display = XOpenDisplay(nullptr);

    QVariant nativeGLHandle = window->openglContext()->nativeHandle();

    if (strcmp(nativeGLHandle.typeName(), "QGLXNativeContext") == 0) {
      QGLXNativeContext nativeContext = qvariant_cast<QGLXNativeContext>(nativeGLHandle);
      //display = nativeContext.display();
      context = nativeContext.context();
      drawable = nativeContext.window();
    }

    int nelements = 0;
    framebufferConfig = glXChooseFBConfig(display, 0, framebufferOptions, &nelements);

    //Enable same-thread context checking
    QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity, false);
}

}
