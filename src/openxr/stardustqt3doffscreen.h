#ifndef STARDUSTQT3DOFFSCREEN_H
#define STARDUSTQT3DOFFSCREEN_H

#include <Qt3DCore>
#include <Qt3DLogic>
#include <Qt3DRender>
#include <QSize>

#include <external/Qt3D-OffscreenRenderer/offscreenengine.h>
#include <external/Qt3D-OffscreenRenderer/offscreenenginedelegate.h>
#include <external/Qt3D-OffscreenRenderer/scenemodifier.h>

#include "vulkan_meta.h"

#include "stardustopenxrgraphics.h"

class StardustQt3DOffscreen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StardustOpenXRGraphics *graphics MEMBER graphics)
    Q_PROPERTY(Qt3DCore::QEntity *sceneRoot MEMBER sceneRoot)

    Q_PROPERTY(Qt3DRender::QCamera *leftEye MEMBER leftEye)
    Q_PROPERTY(Qt3DRender::QCamera *rightEye MEMBER rightEye)
public:
    explicit StardustQt3DOffscreen(QObject *parent = nullptr);
    Q_INVOKABLE void initialize();

    Qt3DCore::QEntity *sceneRoot = nullptr;

    Qt3DRender::QCamera *leftEye = nullptr;
    Qt3DRender::QCamera *rightEye = nullptr;

    QSize eyeDimensions;

    VkImage leftEyeVulkanImage = VK_NULL_HANDLE;
    VkImage rightEyeVulkanImage = VK_NULL_HANDLE;

    StardustOpenXRGraphics *graphics = nullptr;

    Qt3DLogic::QFrameAction *frameAction = nullptr;

public slots:
    void captureFrames(float dt);

private:
    OffscreenEngine* leftEyeEngine;
    OffscreenEngine* rightEyeEngine;

    Qt3DRender::QRenderCaptureReply *leftEyeCapture;
    Qt3DRender::QRenderCaptureReply *rightEyeCapture;

    void onFrameRendered(Qt3DRender::QRenderCaptureReply *capture, VkImage *image);

    VkCommandBuffer beginSingleTimeCommands();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkMemoryRequirements& memRequirements);
private slots:
    void onLeftEyeFrameRendered();
    void onRightEyeFrameRendered();
};

#endif // STARDUSTQT3DOFFSCREEN_H
