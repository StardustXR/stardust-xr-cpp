import QtQuick 2.0
import Qt3D.Core 2.13
import Qt3D.Render 2.13
import Qt3D.Input 2.13
import Qt3D.Extras 2.13
import QtWayland.Compositor 1.3

import Launcher 1.0
import ConfigPathGetter 1.0
import ExtensionLoader 1.0

Entity {
    id:sceneRoot

    Component.onCompleted: {
        autoLauncher.launchDetached("sh -c \""+compositorPrefs.json.autostart.join(";")+"\"");
    }

    Launcher {
        id:autoLauncher
    }

    ConfigPathGetter {
        id: configPathGetter
    }

    components: [
        RenderSettings {
            id:renderSettings

            property alias window: activeFrameGraph.surface
            activeFrameGraph: ForwardRenderer {
                id:startupFrameGraph
                clearColor: Qt.rgba(0, 0.5, 1, 1)
                camera: Camera {
                    fieldOfView: 45
                    projectionType: CameraLens.PerspectiveProjection
                    aspectRatio: 16/9
                    nearPlane: 0.01
                    farPlane: 1000
                    upVector: Qt.vector3d(0,1,0)
                    position: Qt.vector3d(0,0,5)
                }
            }
            pickingSettings.pickMethod: PickingSettings.TrianglePicking
            pickingSettings.faceOrientationPickingMode: PickingSettings.FrontAndBackFace

            Component.onCompleted: {
                var frameGraphComponent = Qt.createComponent("file://"+configPathGetter.loadConfigDirPath("stardust")+"/StardustRenderSettings.qml");
                var newFrameGraph = frameGraphComponent.createObject(this, {});
                activeFrameGraph = newFrameGraph;
                startupFrameGraph.destroy();
            }
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
}
