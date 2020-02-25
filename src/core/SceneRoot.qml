import QtQuick 2.12
import QtQuick3D 1.0

import StardustAPI.Fields 1.0
import StardustAPI.Input 1.0

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
        ModuleLoader.reloadModuleList();
        ModuleLoader.loadAllModules();
    }
}
