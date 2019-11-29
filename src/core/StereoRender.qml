import QtQuick 2.12
import QtQuick3D 1.0

import Stardust.Core 1.0
import Stardust.Core.Internal 1.0

import "../render"

Rectangle {
    anchors.fill: parent

    View3D {
        id: leftView

        renderMode: View3D.Texture

        x: 0
        y: 0
        width: OpenXRGraphics.leftViewSize.width
        height: OpenXRGraphics.leftViewSize.height
    }

    View3D {
        id: rightView

        renderMode: View3D.Texture

        x: OpenXRGraphics.leftViewSize.width
        y: 0
        width: OpenXRGraphics.rightViewSize.width
        height: OpenXRGraphics.rightViewSize.height
    }

    Node {
        id: sceneContents

        SceneRoot {
            id:sceneRoot

            position: Qt.vector3d(0,0,0)

            orientation: Node.RightHanded
            rotationOrder: Node.XYZ
            rotation: Qt.vector3d(-90,0,0)
        }

        StereoCameras {
            id:stereoCameras
        }

        SceneEnvironment {
            id: skybox

            probeBrightness: 1000
            backgroundMode: SceneEnvironment.SkyBox

            lightProbe: Texture {
                source: "qrc:/pond_bridge_night.hdr"
                mappingMode: Texture.Normal

                rotationUV: 180
            }
        }

        Component.onCompleted: {
            leftView.camera = stereoCameras.leftEye;
            leftView.scene = this;
            leftView.environment = skybox;

            rightView.camera = stereoCameras.rightEye;
            rightView.scene = this;
            rightView.environment = skybox;

//            ModuleLoader.getModuleById(ModuleLoader.modules[0].id);
        }
    }
}
