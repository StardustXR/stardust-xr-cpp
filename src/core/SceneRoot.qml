import QtQuick 2.0
import QtQuick3D 1.0
import QtQuick3D.MaterialLibrary 1.0
import QtWayland.Compositor 1.3

import Launcher 1.0
import ConfigPathGetter 1.0
import ExtensionLoader 1.0
import PluginLoader 1.0
import "../render"

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

        position: Qt.vector3d(0,0,0)
        clipFar: 1000
        fieldOfView: 45
        projectionMode: Camera.Perspective
        enableFrustumCulling: true
        rotation: Qt.vector3d(0,0,0)
    }

    Camera {
        id:rightEye

        position: Qt.vector3d(0,0,0)
        clipFar: 1000
        fieldOfView: 45
        projectionMode: Camera.Perspective
        enableFrustumCulling: true
        rotation: Qt.vector3d(0,0,0)
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
