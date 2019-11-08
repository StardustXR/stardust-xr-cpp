#include "stardustopengl.h"

#include <QCoreApplication>
#include <QOpenGLContext>

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

    //Get OpenGL context
    context = glXGetCurrentContext();

    drawable = glXGetCurrentDrawable();

    int nelements = 0;
    framebufferConfig = glXChooseFBConfig(display, 0, framebufferOptions, &nelements);

    //Enable same-thread context checking
    QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity, false);
}

}
