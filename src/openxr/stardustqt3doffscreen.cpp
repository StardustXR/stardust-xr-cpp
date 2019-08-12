#include "stardustqt3doffscreen.h"

StardustQt3DOffscreen::StardustQt3DOffscreen(QObject *parent) : QObject(parent) {

}

void StardustQt3DOffscreen::initialize() {
    //Get the render size from OpenXRGraphics
    eyeDimensions = graphics->eyeDimensions;

    //Create the cameras
    leftEyeEngine = new OffscreenEngine(leftEye, eyeDimensions);
    rightEyeEngine = new OffscreenEngine(rightEye, eyeDimensions);

    //Add the scene root to be rendered to each eye
    leftEyeEngine->setSceneRoot(sceneRoot);
    rightEyeEngine->setSceneRoot(sceneRoot);

    //Create the frameAction and connect it to capturing the frames
    frameAction = new Qt3DLogic::QFrameAction(sceneRoot);
    connect(frameAction, &Qt3DLogic::QFrameAction::triggered, this, &StardustQt3DOffscreen::captureFrames);
}

void StardustQt3DOffscreen::captureFrames(float dt) {
    Q_UNUSED(dt);
//    if(leftEyeCapture != nullptr)
//        delete leftEyeCapture;
//    if(rightEyeCapture != nullptr)
//        delete rightEyeCapture;

    leftEyeCapture = leftEyeEngine->getRenderCapture()->requestCapture();
    rightEyeCapture = rightEyeEngine->getRenderCapture()->requestCapture();
    connect(leftEyeCapture, &Qt3DRender::QRenderCaptureReply::completed, this, &StardustQt3DOffscreen::onLeftEyeFrameRendered);
    connect(rightEyeCapture, &Qt3DRender::QRenderCaptureReply::completed, this, &StardustQt3DOffscreen::onRightEyeFrameRendered);
}

void StardustQt3DOffscreen::onFrameRendered(Qt3DRender::QRenderCaptureReply *capture, VkImage *image) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkDeviceSize imageSize = eyeDimensions.width()*eyeDimensions.height()*4;
    VkMemoryRequirements memRequirements;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, stagingBuffer, stagingBufferMemory, memRequirements);

    capture->image().convertTo(QImage::Format_RGB888);

    const int align_mod = memRequirements.size % memRequirements.alignment;
    const int aligned_size = ((memRequirements.size % memRequirements.alignment) == 0)
                             ? memRequirements.size
                             : (memRequirements.size + memRequirements.alignment - align_mod);

    uint imgData[imageSize];

    void* data = nullptr;
    vkMapMemory(graphics->openxr->vulkan->device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, &imgData, imageSize);
    vkUnmapMemory(graphics->openxr->vulkan->device, stagingBufferMemory);

    void* outData;
    vkMapMemory(graphics->openxr->vulkan->device, stagingBufferMemory, 0, imageSize, 0, &outData);

    vkUnmapMemory(graphics->openxr->vulkan->device, stagingBufferMemory);

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = graphics->vulkanImageTemplateInfo.extent;

    vkCmdCopyBufferToImage(
        commandBuffer,
        stagingBuffer,
        *image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    endSingleTimeCommands(commandBuffer);

    VkSubmitInfo submitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr
    };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(*graphics->openxr->vulkan->queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(*graphics->openxr->vulkan->queue);
}

void StardustQt3DOffscreen::onLeftEyeFrameRendered() {
    onFrameRendered(leftEyeCapture, graphics->leftEyeImage);
}

void StardustQt3DOffscreen::onRightEyeFrameRendered() {
    onFrameRendered(rightEyeCapture, graphics->leftEyeImage);
}

VkCommandBuffer StardustQt3DOffscreen::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = graphics->openxr->vulkan->pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(graphics->openxr->vulkan->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}
void StardustQt3DOffscreen::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkMemoryRequirements& memRequirements) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
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

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = aligned_size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(graphics->openxr->vulkan->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    VkResult result = vkBindBufferMemory(graphics->openxr->vulkan->device, buffer, bufferMemory, 0);
}

uint32_t StardustQt3DOffscreen::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(graphics->openxr->vulkan->physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
}

void StardustQt3DOffscreen::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(*graphics->openxr->vulkan->queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(*graphics->openxr->vulkan->queue);

    vkFreeCommandBuffers(graphics->openxr->vulkan->device, graphics->openxr->vulkan->pool, 1, &commandBuffer);
}
