import QtQuick 2.6
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtQuick.Scene2D 2.12
import QtWayland.Compositor 1.3

Entity {
    property real ppm: 250
    property size dimensions: Qt.size(1000, 1000)
    property real resolution: 2
    property bool flipped: false
    property variant position: Qt.vector3d(0,0,0)
    property real rotX: 0
    property real rotY: 0
    property real rotZ: 0

    property ShellSurface shellSurf
    property real listIndex

    id:panel

    components: [panelTransform, planeMesh, material, panelPicker]


    Mesh {
        id:planeMesh
        source: "qrc:/Plane.ply"
    }

    Transform {
        id: panelTransform
        translation: position
        scale3D: shellSurf != null ? Qt.vector3d(shellSurf.surface.size.width/ppm, shellSurf.surface.size.height/ppm, 1) : Qt.vector3d(0,0,0)
        rotationX: rotX
        rotationY: rotY
        rotationZ: rotZ
    }

    NormalDiffuseMapAlphaMaterial {
        id:material
        ambient: "white"
        diffuse: texture
        normal: Texture2D {
            TextureImage {
                source: "qrc:/normal_map_flat.png"
            }
        }
        shininess: 0
    }

//    PhongMaterial {
//        id:material
//        diffuse: "white"
//    }

    ObjectPicker {
        id: panelPicker
        hoverEnabled: true
        dragEnabled: true
    }

    NumberAnimation on rotY {
        from:0; to:360
        duration: 10000
        easing.type: Easing.Linear
        loops: Animation.Infinite
        running: true
    }

    Scene2D {
        id: waylandScene
        output: RenderTargetOutput {
            attachmentPoint: RenderTargetOutput.Color0
            texture: Texture2D {
                id: texture
                height: shellSurf != null ? shellSurf.surface.size.height : 128
                width: shellSurf != null ? shellSurf.surface.size.width : 128
                format: Texture.RGBA8_UNorm
                minificationFilter: Texture.Linear
                magnificationFilter: Texture.Nearest
            }
        }

        entities: [panel]

        mouseEnabled: true

        ShellSurfaceItem {
            id:waylandQuickItem
            height: shellSurf != null ? shellSurf.surface.size.height : 128
            width: shellSurf != null ? shellSurf.surface.size.width : 128
            shellSurface: shellSurf
            onSurfaceDestroyed: function() {
                panel.destroy();
                shellSurfaces.remove(listIndex);
            }
        }
    }


    Timer {
        onTriggered: function() {
            console.log("shellSurf:");
            console.log(shellSurf);
        }
        running: true
        interval: 1000
        repeat: true
    }
}
