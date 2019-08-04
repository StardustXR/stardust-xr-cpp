#include "stardustopenxr.h"

StardustOpenXR::StardustOpenXR(QObject *parent) : QObject(parent) {
}

void StardustOpenXR::initialize() {
    //XrApplicationInfo *xrAppInfo
    strcpy(xrAppInfo->applicationName, QString("Stardust XR").toUtf8());
    xrAppInfo->applicationVersion = 1;
    strcpy(xrAppInfo->engineName, QString("Qt").toUtf8());
    xrAppInfo->engineVersion = 5;
    xrAppInfo->apiVersion = XR_CURRENT_API_VERSION;

    //XrInstanceCreateInfo *xrInstanceInfoXrInstanceCreateInfo *xrInstanceInfo
    xrInstanceInfo->type = XR_TYPE_INSTANCE_CREATE_INFO;
    xrInstanceInfo->next = nullptr;
    xrInstanceInfo->createFlags = 0;
    xrInstanceInfo->applicationInfo = *xrAppInfo;
    xrInstanceInfo->enabledApiLayerCount = 0;
    xrInstanceInfo->enabledExtensionCount = 0;

    //Create the OpenXR Instance
    xrCreateInstance(xrInstanceInfo, xrInstance);

    //XrSystemGetInfo
    hmdInfo->type = XR_TYPE_SYSTEM_GET_INFO;
    hmdInfo->next = nullptr;
    hmdInfo->formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

    //XrSystemId *hmdID
    xrGetSystem(*xrInstance, hmdInfo, hmdID);

    //XrSessionCreateInfo *xrSessionInfo
    xrSessionInfo->type = XR_TYPE_SESSION_CREATE_INFO;
    xrSessionInfo->next = nullptr;
    xrSessionInfo->systemId = *hmdID;
    xrSessionInfo->createFlags = 0;

    //XrSession *stardustSession;
    xrCreateSession(*xrInstance, xrSessionInfo, stardustSession);
}
