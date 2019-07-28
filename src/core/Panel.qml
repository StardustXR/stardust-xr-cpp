import QtQuick 2.6
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtQuick.Scene2D 2.12

import PassthroughKeyboardHandler 1.0

Entity {
    property real ppm: 250
    property size dimensions: Qt.size(1000, 1000)
    property variant position: Qt.vector3d(0,0,0)
    property real rotX: 0
    property real rotY: 0
    property real rotZ: 0

    property Item panelContents

    property real margin: 0
    property real marginTop: margin
    property real marginBottom: margin
    property real marginLeft: margin
    property real marginRight: margin

    property size croppedDimensions: Qt.size(dimensions.width-marginLeft-marginRight, dimensions.height-marginTop-marginBottom)

    property alias mouseEnabled: panelScene.mouseEnabled

    function onClicked() {}
    function onPressed() {}
    function onReleased() {}

    id:panel

    components: [panelTransform, planeMesh, material, panelPicker]


    Mesh {
        id:planeMesh
        source: "qrc:/Plane.ply"
    }

    Transform {
        id: panelTransform
        translation: position
        scale3D: Qt.vector3d(croppedDimensions.width/ppm, 1, croppedDimensions.height/ppm)
        rotationX: rotX
        rotationY: rotY
        rotationZ: rotZ
    }

    NormalDiffuseMapAlphaMaterial {
        id:material
        ambient: "white"
        diffuse: surfaceTexture
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
        onClicked: parent.onClicked()
        onPressed: parent.onPressed()
        onReleased: parent.onReleased()
    }


    Scene2D {
        id: panelScene
        output: RenderTargetOutput {
            id: sceneRenderTargetOutput
            attachmentPoint: RenderTargetOutput.Color0
            texture: Texture2D {
                id: surfaceTexture
                height: croppedDimensions.height
                width: croppedDimensions.width
                format: Texture.RGBA8_UNorm
                generateMipMaps: true
                magnificationFilter: Texture.Linear
                minificationFilter: Texture.LinearMipMapLinear
                wrapMode {
                    x: WrapMode.ClampToEdge
                    y: WrapMode.ClampToEdge
                }
            }
        }
        mouseEnabled: true
        entities: []

        Item {
            id:panelContentsContainer
            height: dimensions.height
            width: dimensions.width

            x: -marginLeft
            y: -marginTop

            children: [panelContents]
        }
    }
    PassthroughKeyboardHandler {
        target: panelContents

        Component.onCompleted: {
            physicalKeyboardAdapter.fullKeyEvent.connect(this.fullKeyEvent);
        }
    }


    Timer {
        onTriggered: function() {
            panelScene.entities = [parent];
            console.log(panelContents.height);
            console.log(panelContents.width);
        }
        running: true
        interval: 250
        repeat: false
    }
}
