#ifndef STARDUSTOPENXRFRAME_H
#define STARDUSTOPENXRFRAME_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "stardustopenxrgraphics.h"
#include <QElapsedTimer>

namespace Stardust {

class OpenXRFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OpenXRGraphics *graphics MEMBER graphics)
public:
    explicit OpenXRFrame(QObject *parent = nullptr);

    OpenXRGraphics *graphics = nullptr;
    QThread *thread = nullptr;

public slots:
    void initialize();

private slots:
    void startFrame();

    void renderFrame();

    void endFrame();

private:
    void copyFrame(uint i);
    void initRenderControl();

    //OpenGL helper functions
    void createTextureFromFD(int d);

    //OpenGL variables
    GLuint copyFBO;
    GLuint colorTex;

    //OpenXR declarations
    XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO, nullptr};
    XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, nullptr};

    //FPS Timing
    QElapsedTimer *frameTimer;
    float fps = 0;
};

}

#endif // STARDUSTOPENXRFRAME_H
