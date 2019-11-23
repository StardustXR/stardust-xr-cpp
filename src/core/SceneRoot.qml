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

    Model {
        source: "qrc:/Teapot.mesh"

        position: Qt.vector3d(0, 0.5, -1.65)

        materials: [
            DefaultMaterial {
                lighting: DefaultMaterial.NoLighting

                emissiveColor: "#00ff7f"
                emissiveFactor: 5
            }
        ]
    }
}
