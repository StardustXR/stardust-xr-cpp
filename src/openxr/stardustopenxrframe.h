#ifndef STARDUSTOPENXRFRAME_H
#define STARDUSTOPENXRFRAME_H


#include "openxr_meta.h"
#include "stardustopenxrgraphics.h"

namespace Stardust {

class OpenXRFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OpenXRGraphics *graphics MEMBER graphics)
public:
    explicit OpenXRFrame(QObject *parent = nullptr);

    OpenXRGraphics *graphics = nullptr;
    QThread *thread = nullptr;

signals:
    void initialized();

    void startedFrame();

    void renderedFrame();

    void frameEnded();

public slots:
    void initialize();

private slots:
    void startFrame();

    void renderFrame();

    void endFrame();

private:
    void copyFrame(uint i);
    void initRenderControl();

    //Vulkan helper functions
    VkCommandBuffer beginSingleTimeCommands(uint32_t count);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createEXTBuffers();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory, int &fd, VkMemoryRequirements &memRequirements);

    //Vulkan variables
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkDeviceSize imageSize;
    VkMemoryRequirements memRequirements;
    int fd = 0;

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
