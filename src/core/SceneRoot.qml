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

    Camera {
        id:leftEye

        clipNear: 0.001
        clipFar: 1000
        fieldOfView: 110
        projectionMode: Camera.Perspective
//        enableFrustumCulling: true

        Component.onCompleted: OpenXRGraphics.leftEye = this
    }

    Camera {
        id:rightEye

        clipNear: 0.001
        clipFar: 1000
        fieldOfView: 110
        projectionMode: Camera.Perspective
//        enableFrustumCulling: true

        Component.onCompleted: OpenXRGraphics.rightEye = this
    }

    SceneEnvironment {
        id: skybox
        clearColor: "skyblue"
        probeBrightness: 1000
        backgroundMode: SceneEnvironment.SkyBox

        lightProbe: Texture {
            source: "qrc:/test_grid.png"
            mappingMode: Texture.Environment
        }
    }
}
