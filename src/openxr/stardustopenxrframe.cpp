#include "stardustopenxrframe.h"
#include <QtQuick3D/private/qquick3dcamera_p.h>
#include <QDebug>

#define RAD2DEG 180/3.14159

StardustOpenXRFrame::StardustOpenXRFrame(QObject *parent) : QObject(parent) {
    connect(this, &StardustOpenXRFrame::frameEnded, this, &StardustOpenXRFrame::startFrame);

    connect(this, &StardustOpenXRFrame::startedFrame, this, &StardustOpenXRFrame::renderFrame);

    connect(this, &StardustOpenXRFrame::renderedFrame, this, &StardustOpenXRFrame::endFrame);
}

void StardustOpenXRFrame::startFrame() {
    qDebug() << "Starting frame";
    //Wait for next frame
    xrWaitFrame(*graphics->openxr->stardustSession, &graphics->frameWaitInfo, &graphics->frameState);

    //Begin drawing a new frame
    xrBeginFrame(*graphics->openxr->stardustSession, nullptr);

    //Update the parent's FPS value
//        qDebug() << "FPS:" << 1000000000/frameState.predictedDisplayTime;
    graphics->displayFPS = static_cast<uint>(1000000000/graphics->frameState.predictedDisplayTime);

    //Do all this for both eyes
    for(int i=0; i<2; i++) {
        //Create acquire information
        XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO, nullptr};

        //Grab the swapchain image
        xrAcquireSwapchainImage(graphics->swapchains[i], &acquireInfo, &graphics->swapchainImageIndices.at(i));

        //Create empty swapchain wait info
        XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, nullptr};

        //Wait for when the swapchain images are ready to be written
        xrWaitSwapchainImage(graphics->swapchains[i], &waitInfo);
    }

    //Update view information
    graphics->viewLocateInfo.viewConfigurationType = graphics->openxr->viewConfig;
    graphics->viewLocateInfo.displayTime = graphics->frameState.predictedDisplayTime;
    graphics->viewLocateInfo.space = graphics->refSpace;

    //Locate views
    xrLocateViews(*graphics->openxr->stardustSession, &graphics->viewLocateInfo, &graphics->viewState, 2, nullptr, graphics->views.data());

    //Do for each eye
    for(int i=0; i<2; i++) {
        QQuick3DCamera *eye = qobject_cast<QQuick3DCamera *>(graphics->leftEye);
        if(i==1)
            eye = qobject_cast<QQuick3DCamera *>(graphics->rightEye);

        XrView &view = graphics->views[i];
        //Set the cameras' positon to the pose position
        QVector3D position = QVector3D(view.pose.position.x, view.pose.position.z, view.pose.position.y);
        eye->setPosition(position);

        //Set the cameras' orientation to match the orientation
        QQuaternion rotation = QQuaternion(view.pose.orientation.w, view.pose.orientation.x, view.pose.orientation.y, view.pose.orientation.z);
        QVector3D euler = rotation.toEulerAngles();
        eye->setRotation(-euler);

        eye->setFieldOfView((view.fov.angleRight-view.fov.angleLeft)*RAD2DEG);

        eye->setFrustumTop(view.fov.angleUp*RAD2DEG);
        eye->setFrustumBottom(view.fov.angleDown*RAD2DEG);
        eye->setFrustumLeft(view.fov.angleLeft*RAD2DEG);
        eye->setFrustumRight(view.fov.angleRight*RAD2DEG);

        //Update properties on the Xr`rameEndInfo and its dependencies
        graphics->stardustLayerViews[i].fov = view.fov;
        graphics->stardustLayerViews[i].pose = view.pose;
        graphics->stardustLayerViews[i].subImage = XrSwapchainSubImage {
            graphics->swapchains[i],
            graphics->eyeRects[i],
            graphics->swapchainImageIndices[i]
        };
    }

    emit startedFrame();
}

void StardustOpenXRFrame::renderFrame() {
    qDebug() << "Rendering frame";

    graphics->surfaces[0]->setProperty("disableAutomaticRender", true);
    GLuint leftTex;
    QMetaObject::invokeMethod(graphics->surfaces[0],
                              "render",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(GLuint, leftTex)
            );

    copyFrame(0, leftTex, graphics->vulkanImages[0][0]);
    copyFrame(1, 0, graphics->vulkanImages[1][0]);

    emit renderedFrame();
}


void StardustOpenXRFrame::endFrame() {
    qDebug() << "Ending frame";
    //Update properties on the XrFrameEndInfo and its dependencies
    XrCompositionLayerProjection stardustLayer = {
        XR_TYPE_COMPOSITION_LAYER_PROJECTION,
        nullptr,
        0,
        graphics->refSpace,
        2,
        graphics->stardustLayerViews
    };
    stardustLayer.views = graphics->stardustLayerViews;
    std::vector<XrCompositionLayerBaseHeader*> layers;
    if(graphics->frameState.shouldRender) {
        layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&stardustLayer));
    }
    XrFrameEndInfo endInfo = {
        XR_TYPE_FRAME_END_INFO,
        nullptr,
        graphics->frameState.predictedDisplayTime,
        XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
        static_cast<uint32_t>(layers.size()),
        layers.data()
    };
    //Create release information
    XrSwapchainImageReleaseInfo releaseInfo = {XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO, nullptr};

    //Release the swapchain images
    xrReleaseSwapchainImage(graphics->swapchains[0], &releaseInfo);
    xrReleaseSwapchainImage(graphics->swapchains[1], &releaseInfo);

    //End the drawing of the current frame
    xrEndFrame(*graphics->openxr->stardustSession, &endInfo);

    emit frameEnded();
}






//Vulkan shortcuts

void StardustOpenXRFrame::copyFrame(int i, GLuint glTexID, VkImage image) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(3);
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkDeviceSize imageSize = graphics->eyeDimensions.width()*graphics->eyeDimensions.height()*4;
    VkMemoryRequirements memRequirements;
    int fd = 0;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, stagingBuffer, stagingBufferMemory, fd, memRequirements);

    VkImageSubresourceRange range;
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseArrayLayer = 0;
    range.layerCount = 1;
    range.baseMipLevel = 0;
    range.levelCount = 1;

    VkImageMemoryBarrier layoutTransition = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        graphics->openxr->vulkan->queueFamilyIndex,
        graphics->openxr->vulkan->queueFamilyIndex,
        image,
        range
    };

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &layoutTransition);

    const int align_mod = memRequirements.size % memRequirements.alignment;
    const int aligned_size = ((memRequirements.size % memRequirements.alignment) == 0)
                             ? memRequirements.size
                             : (memRequirements.size + memRequirements.alignment - align_mod);

    uint *imgData = (uint*) malloc(sizeof (uint) * imageSize); // TODO: free somewhere

    memset(imgData, 255, sizeof(uint) * imageSize);

    void* data = nullptr;
    vkMapMemory(graphics->openxr->vulkan->device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, imgData, imageSize);
    vkUnmapMemory(graphics->openxr->vulkan->device, stagingBufferMemory);

    free(imgData);

    VkBufferImageCopy region = {};
    region.bufferOffset = align_mod;
    region.bufferRowLength = graphics->eyeDimensions.width();
    region.bufferImageHeight = graphics->eyeDimensions.height();

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = graphics->imageExtents[i];

    vkCmdCopyBufferToImage(
        commandBuffer,
        stagingBuffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    layoutTransition.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    layoutTransition.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    layoutTransition.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    layoutTransition.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &layoutTransition);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr
    };
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer cmdBuffers[1] = {commandBuffer};
    submitInfo.pCommandBuffers = cmdBuffers;

    vkQueueSubmit(*graphics->openxr->vulkan->queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(*graphics->openxr->vulkan->queue);
}

VkCommandBuffer StardustOpenXRFrame::beginSingleTimeCommands(uint32_t count) {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = graphics->openxr->vulkan->pool;
    allocInfo.commandBufferCount = count;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(graphics->openxr->vulkan->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}
void StardustOpenXRFrame::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, int &fd, VkMemoryRequirements& memRequirements) {
    VkExternalMemoryBufferCreateInfo externalBufferInfo = {
        VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO,
        nullptr,
        VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT
    };

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//    bufferInfo.pNext = &externalBufferInfo;
    bufferInfo.pNext = nullptr;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 1;
    bufferInfo.pQueueFamilyIndices = &graphics->openxr->vulkan->queueFamilyIndex;

    if (vkCreateBuffer(graphics->openxr->vulkan->device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    vkGetBufferMemoryRequirements(graphics->openxr->vulkan->device, buffer, &memRequirements);

    const int align_mod = memRequirements.size % memRequirements.alignment;
    const int aligned_size = ((memRequirements.size % memRequirements.alignment) == 0)
                             ? memRequirements.size
                             : (memRequirements.size + memRequirements.alignment - align_mod);


    VkImportMemoryFdInfoKHR import_memory_info = {
        VK_STRUCTURE_TYPE_IMPORT_MEMORY_FD_INFO_KHR,
        nullptr,
        VK_EXTERNAL_MEMORY_HANDLE_TYPE_DMA_BUF_BIT_EXT,
        fd
    };

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//    allocInfo.pNext = &import_memory_info;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = aligned_size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(graphics->openxr->vulkan->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    VkResult result = vkBindBufferMemory(graphics->openxr->vulkan->device, buffer, bufferMemory, 0);
}

uint32_t StardustOpenXRFrame::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(graphics->openxr->vulkan->physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
}


