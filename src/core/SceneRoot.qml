import QtQuick 2.0
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Logic 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtWayland.Compositor 1.3

import Launcher 1.0
import ConfigPathGetter 1.0
import ExtensionLoader 1.0
import PluginLoader 1.0
import "../render"
import OpenXRGraphics 1.0
import Qt3D.Offscreen 1.0

Entity {
    id:sceneRoot

    property alias leftEye: leftEye
    property alias rightEye: rightEye

    Component.onCompleted: {
        autoLauncher.launchDetached("sh -c \""+compositorPrefs.json.autostart.join(";")+"\"");
    }

    Launcher {
        id:autoLauncher
    }

    ConfigPathGetter {
        id: configPathGetter
    }

    Camera {
        id:leftEye

        onViewCenterChanged: console.log(viewCenter.minus(position));
    }

    Camera {
        id:rightEye

        onViewCenterChanged: console.log(viewCenter.minus(position));
    }

    components: [
        StereoRenderSettings {
            id:renderSettings
        },
        InputSettings {}
    ]

    TestSphere {
        position: leftEye.position
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
