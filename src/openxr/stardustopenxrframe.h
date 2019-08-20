#ifndef STARDUSTOPENXRFRAME_H
#define STARDUSTOPENXRFRAME_H

#include <QObject>

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
    void renderFrame();

    void copiedLeftView();
    void copiedRightView();

    void frameEnded();

public slots:
    void startFrame();

private slots:
    void copyLeftView();
    void copyRightView();

    void endFrame();

private:
    void copyFrame(QQuick3DViewport *view, VkImage *image);

    //Vulkan helper functions
    VkCommandBuffer beginSingleTimeCommands(uint32_t count);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory, VkMemoryRequirements &memRequirements);
};

#endif // STARDUSTOPENXRFRAME_H
