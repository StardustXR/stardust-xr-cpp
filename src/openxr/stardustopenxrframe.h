#ifndef STARDUSTOPENXRFRAME_H
#define STARDUSTOPENXRFRAME_H


#include "openxr_meta.h"
#include "stardustopenxrgraphics.h"

class StardustOpenXRFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StardustOpenXRGraphics *graphics MEMBER graphics)
public:
    explicit StardustOpenXRFrame(QObject *parent = nullptr);

    StardustOpenXRGraphics *graphics = nullptr;
    QThread *thread = nullptr;

signals:
    void startedFrame();

    void renderedFrame();

    void frameEnded();

public slots:
    void startFrame();

private slots:
    void renderFrame();

    void endFrame();

private:
    void copyFrame(int i, GLuint glTexID, VkImage image);

    //Vulkan helper functions
    VkCommandBuffer beginSingleTimeCommands(uint32_t count);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory, int &fd, VkMemoryRequirements &memRequirements);
};

#endif // STARDUSTOPENXRFRAME_H
