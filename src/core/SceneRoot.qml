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

    position: Qt.vector3d(0,0,0)

    property alias leftEye: leftEye
    property alias rightEye: rightEye

    Component.onCompleted: {
        autoLauncher.launchDetached("sh -c \""+compositorPrefs.json.autostart.join(";")+"\"");
    }

    Launcher {
        id:autoLauncher
    }

    Camera {
        id:leftEye
    }

    Camera {
        id:rightEye
    }
}
