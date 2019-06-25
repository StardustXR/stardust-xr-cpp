import QtQuick 2.0
import Qt3D.Render 2.12
import Qt3D.Extras 2.12

ForwardRenderer {
    id:renderer
    clearColor: Qt.rgba(0, 0, 0, 1)
    camera: Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 100.0
        position: Qt.vector3d(0,0,5)
        upVector: Qt.vector3d(0,0,1)
        viewCenter: Qt.vector3d(0,0,0)
    }
}
