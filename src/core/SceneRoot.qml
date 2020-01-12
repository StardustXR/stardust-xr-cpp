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
        ModuleLoader.reloadModuleList();
        ModuleLoader.loadAllModules();

        var component = ModuleLoader.modules[0].components[0];
        if(component.status === Component.Error) {
            console.log(component.errorString());
        }

        if(!instantiateComponent(component, modules))
            component.onCompleted = instantiateComponent(component, modules);
    }

    function instantiateComponent(component, parent) {
        var componentReady = component.status === Component.Ready;
        if(componentReady)
            component.createObject(parent);
        return componentReady;
    }

    Node {
        id:modules
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
