import QtQuick 2.12
import QtQuick3D 1.0

import OpenXRGraphics 1.0

Node {
    property alias leftEye: leftEye
    property alias rightEye: rightEye

    Camera {
        id:leftEye

        clipNear: 0.001
        clipFar: 1000
        fieldOfView: 110
        projectionMode: Camera.Frustum
        enableFrustumCulling: true

        Component.onCompleted: OpenXRGraphics.leftEye = this
    }

    Camera {
        id:rightEye

        clipNear: 0.001
        clipFar: 1000
        fieldOfView: 110
        projectionMode: Camera.Frustum
        enableFrustumCulling: true

        Component.onCompleted: OpenXRGraphics.rightEye = this
    }
}
