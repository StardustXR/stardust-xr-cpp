import QtQuick 2.0
import QtQuick3D 1.0
import QtQuick3D.MaterialLibrary 1.0

import Launcher 1.0
import ConfigPathGetter 1.0
import ExtensionLoader 1.0
import PluginLoader 1.0
import "../render"

import OpenXRGraphics 1.0

Node {
    id:sceneRoot

    property alias leftEye: leftEye
    property alias rightEye: rightEye
    property alias skybox: skybox

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

    Camera {
        id:leftEye

        clipNear: 0.001
        clipFar: 1000
        fieldOfView: 110
        projectionMode: Camera.Frustum
        enableFrustumCulling: true

        Component.onCompleted: OpenXRGraphics.leftEye = this
        onPositionChanged: console.log(position)
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
}
