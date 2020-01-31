import QtQuick 2.12
import QtQuick3D 1.0

import Stardust.Core 1.0
//import Stardust.Core.Internal 1.0

import QtQuick3D.OpenXR 1.0

import "../render"

Node {
    id:sceneRoot

    SceneEnvironment {
        id: skybox

        probeBrightness: 1000
        backgroundMode: SceneEnvironment.SkyBox

        lightProbe: Texture {
            source: "qrc:/pond_bridge_night.hdr"
        }

        Component.onCompleted: OpenXR.setEnvironment(skybox)
    }

    Component.onCompleted: {
        ModuleLoader.setParentItem(this);
    }

    Model {
        source: "qrc:/Teapot.mesh"

        position: Qt.vector3d(0, 0.5, -1.65)

        materials: [
            DefaultMaterial {
                lighting: DefaultMaterial.NoLighting

                emissiveColor: "#00ff7f"
            }
        ]
    }
}
