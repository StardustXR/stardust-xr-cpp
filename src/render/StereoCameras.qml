import QtQuick 2.12
import QtQuick3D 1.0

import Stardust.Core.Internal 1.0

Node {
    property alias leftEye: leftEye
    property alias rightEye: rightEye

    FrustumCamera {
        id:leftEye

        clipNear: 0.001
        clipFar: 1000
        fieldOfView: 110

        Component.onCompleted: OpenXRGraphics.leftEye = this
    }

    FrustumCamera {
        id:rightEye

        clipNear: 0.001
        clipFar: 1000
        fieldOfView: 110

        Component.onCompleted: OpenXRGraphics.rightEye = this
    }
}
