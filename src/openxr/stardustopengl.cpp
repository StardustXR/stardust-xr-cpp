#include "stardustopengl.h"

namespace Stardust {

OpenGL::OpenGL(QObject *parent) : QObject(parent) {

}

void OpenGL::initialize() {
    //Temporarily make the context current
    QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity, true);
    window->openglContext()->doneCurrent();
    window->openglContext()->makeCurrent(window);

    //Get native context for window
    nativeInterface = QGuiApplication::platformNativeInterface();

    //Get the ideal framebuffer
//    std::vector<int> attrib_list = {
////        GLX_X_RENDERABLE, false,
////        GLX_TRANSPARENT_TYPE, GLX_TRANSPARENT_RGB,
//        GLX_RED_SIZE, 8,
//        GLX_GREEN_SIZE, 8,
//        GLX_BLUE_SIZE, 8,
//        GLX_ALPHA_SIZE, 8,
//        None
//    };
//    int numElements;
//    const GLXFBConfig *framebufferConfigs = glXChooseFBConfig(display, 0, attrib_list.data(), &numElements);
//    framebufferConfig = framebufferConfigs[0];

//    //Get OpenGL context
//    context = glXGetCurrentContext();

//    drawable = glXGetCurrentDrawable();

    //Enable same-thread context checking
    QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity, false);
}

}
