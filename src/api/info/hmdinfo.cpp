#include "hmdinfo.h"

namespace StardustAPI {
namespace Info {

HmdInfo::HmdInfo(QOpenXRApplication *oxr) {
    this->oxr = oxr;

    connect(oxr, &QOpenXRApplication::gazePositionChanged, this, &HmdInfo::oxrGazePositionChanged);
    connect(oxr, &QOpenXRApplication::gazeDirectionChanged, this, &HmdInfo::oxrGazeDirectionChanged);
}

QVector3D HmdInfo::gazePosition() {
    return oxr->getGazePosition();
}
void HmdInfo::oxrGazePositionChanged() {
    emit gazePositionChanged();
}

QVector3D HmdInfo::gazeDirection() {
    return oxr->getGazeDirection();
}
void HmdInfo::oxrGazeDirectionChanged() {
    emit gazeDirectionChanged();
}

} // namespace Info
} // namespace StardustAPI
