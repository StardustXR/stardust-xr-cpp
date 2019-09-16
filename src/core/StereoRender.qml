import QtQuick 2.12
import QtQuick3D 1.0

import Stardust.Core 1.0
import Stardust.Core.Internal 1.0

Rectangle {
    anchors.fill: parent
    View3D {
        id: leftView

        renderMode: View3D.Texture

        x: 0
        y: 0
        width: OpenXRGraphics.leftViewSize.width
        height: OpenXRGraphics.leftViewSize.height

//        camera: sceneRoot.leftEye
//        scene: sceneRoot
//        environment: sceneRoot.skybox
    }

    View3D {
        id: rightView

        renderMode: View3D.Texture

        x: OpenXRGraphics.leftViewSize.width
        y: 0
        width: OpenXRGraphics.rightViewSize.width
        height: OpenXRGraphics.rightViewSize.height

//        camera: sceneRoot.rightEye
//        scene: sceneRoot
//        environment: sceneRoot.skybox
    }

    Loader {
        source: "SceneRoot.qml"
    }

//    Preferences {
//        id:appPrefs
//        fileName: "app_prefs.json"
//    }

//    Preferences {
//        id:compositorPrefs
//        fileName: "compositor_prefs.json"
//    }
}
