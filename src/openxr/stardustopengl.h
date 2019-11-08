#ifndef STARDUSTOPENGL_H
#define STARDUSTOPENGL_H

#include <QQuickWindow>

#include "openxr_meta.h"

namespace Stardust {

class OpenXR;

class OpenGL : public QObject
{
    Q_OBJECT
public:
    explicit OpenGL(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();
    QQuickWindow *window;

    Display *display;

    GLXContext context;
    GLXDrawable drawable;
    GLXFBConfig *framebufferConfig;

    int framebufferOptions[11] = {GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, None};

    OpenXR *openxr;
};

}


#endif // STARDUSTOPENGL_H
