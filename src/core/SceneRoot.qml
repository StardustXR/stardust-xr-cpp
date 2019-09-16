import QtQuick 2.12
import QtQuick3D 1.0

import Stardust.Core 1.0
import Stardust.Core.Internal 1.0

import "../render"

Node {
    id:sceneRoot

//    Component.onCompleted: {
//        autoLauncher.launchDetached("sh -c \""+compositorPrefs.json.autostart.join(";")+"\"");
//    }

//    Launcher {
//        id:autoLauncher
//    }



    Light {
        id: directionalLight
//        type: Light.Directional
    }

    Model {
        source: "#Cube"

        position: Qt.vector3d(0, 1.65, 5)
        scale: Qt.vector3d(1,1,1)

        materials: [
            DefaultMaterial {
                id: material
                diffuseColor: "white"
            }

        ]
    }

    StereoCameras {
        id:stereoCameras
    }

    SceneEnvironment {
        id: skybox
        clearColor: "skyblue"
        probeBrightness: 1000
        backgroundMode: SceneEnvironment.SkyBox

        lightProbe: Texture {
            source: "qrc:/pond_bridge_night.hdr"
            mappingMode: Texture.Environment
        }
    }

    Component.onCompleted: {
        leftView.camera = stereoCameras.leftEye;
        leftView.scene = this;
        leftView.environment = skybox;

        rightView.camera = stereoCameras.rightEye;
        rightView.scene = this;
        rightView.environment = skybox;
    }
}
