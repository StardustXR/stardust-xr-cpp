import QtQuick 2.0
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtQuick.Scene2D 2.12
import QtWayland.Compositor 1.3
import Launcher 1.0
import ExtensionLoader 1.0

Entity {
    id:sceneRoot

    Component.onCompleted: {
        autoLauncher.launchDetached("sh -c \""+compositorPrefs.json.autostart.join(";")+"\"");
    }

    Launcher {
        id:autoLauncher
    }

    Camera {
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

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                id:renderer
                clearColor: Qt.rgba(0, 0.5, 1, 1)
                camera: camera
            }
            pickingSettings.pickMethod: PickingSettings.TrianglePicking
            pickingSettings.faceOrientationPickingMode: PickingSettings.FrontAndBackFace
        },
        InputSettings {}
    ]

    FirstPersonCameraController {
        camera: camera
    }

    NodeInstantiator {
        model: shellSurfaces

        Panel {
            id:panel
            shellSurf: modelData
            listIndex: index
        }
    }

    QMLExtensionLoader {
        extensionType: "keyboard"
    }
}
