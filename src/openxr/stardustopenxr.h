#ifndef STARDUSTOPENXR_H
#define STARDUSTOPENXR_H

#include <QObject>
#include <openxr/openxr.h>

class StardustOpenXR : public QObject
{
    Q_OBJECT
public:
    explicit StardustOpenXR(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();

    XrApplicationInfo *xrAppInfo = new XrApplicationInfo;
    XrInstanceCreateInfo *xrInstanceInfo = new XrInstanceCreateInfo;
    XrInstance *xrInstance = new XrInstance;
    XrSystemGetInfo *hmdInfo = new XrSystemGetInfo;
    XrSystemId *hmdID = new XrSystemId;
    XrSessionCreateInfo *xrSessionInfo = new XrSessionCreateInfo;
    XrSession *stardustSession = new XrSession;
};

#endif // STARDUSTOPENXR_H
