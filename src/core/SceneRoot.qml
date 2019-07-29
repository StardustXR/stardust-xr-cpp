import QtQuick 2.0
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtWayland.Compositor 1.3

import Launcher 1.0
import ConfigPathGetter 1.0
import ExtensionLoader 1.0
import PluginLoader 1.0
import "../render"

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

    MonoCamera {
        id: monoCamera
        lens: CameraLens {
            fieldOfView: 60
            projectionType: CameraLens.PerspectiveProjection
            aspectRatio: displaySurface.width/displaySurface.height
            nearPlane: 0.01
            farPlane: 1000
        }
        position: Qt.vector3d(0,-0.5,0)
        viewVector: Qt.vector3d(0,1,0)

    }

    components: [
        RenderSettings {
            id:renderSettings

            activeFrameGraph: ForwardRenderer {
                id:startupFrameGraph
                clearColor: Qt.rgba(0, 0.5, 1, 1)
                camera: monoCamera
            }
            pickingSettings.pickMethod: PickingSettings.TrianglePicking
            pickingSettings.faceOrientationPickingMode: PickingSettings.FrontAndBackFace

            Component.onCompleted: {
                var frameGraphComponent = Qt.createComponent("file://"+configPathGetter.loadConfigDirPath("stardust")+"/StardustRenderSettings.qml");
                var newFrameGraph = frameGraphComponent.createObject(this, {"monoCam": monoCamera});
                activeFrameGraph = newFrameGraph;
                startupFrameGraph.destroy();
            }
        },
        InputSettings {}
    ]

    TestSphere {
        position: monoCamera.position
    }

    NodeInstantiator {
        model: shellSurfaces

        WaylandPanel {
            id:panel
            shellSurf: modelData
            listIndex: index
        }
    }
}
