import QtQuick 2.6
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtQuick.Scene2D 2.12
import QtWayland.Compositor 1.3
import QtQuick.Controls 2.5

import Launcher 1.0
import WaylandKeyboardHandler 1.0

Entity {
    property real ppm: loadAppPref("ppm", 250)
    property size dimensions: Qt.size(1000, 1000)
    property variant position: Qt.vector3d(0,0,0)
    property real rotX: 0
    property real rotY: 0
    property real rotZ: 0

    property ShellSurface shellSurf
    property real listIndex
    property string processName

    property real margin: loadAppPref("margin", 0)
    property real marginTop: loadAppPref("marginTop", margin)
    property real marginBottom: loadAppPref("marginBottom", margin)
    property real marginLeft: loadAppPref("marginLeft", margin)
    property real marginRight: loadAppPref("marginRight", margin)

    id:panel

    components: [panelTransform, planeMesh, material, panelPicker]


    Mesh {
        id:planeMesh
        source: "qrc:/res/Plane.ply"
    }

    Transform {
        id: panelTransform
        translation: position
        scale3D: shellSurf != null ? Qt.vector3d(waylandSurfaceTexture.width/ppm, 1, waylandSurfaceTexture.height/ppm) : Qt.vector3d(0,0,0)
        rotationX: rotX
        rotationY: rotY
        rotationZ: rotZ
    }

    NormalDiffuseMapAlphaMaterial {
        id:material
        ambient: "white"
        diffuse: waylandSurfaceTexture
        normal: Texture2D {
            TextureImage {
                source: "qrc:/res/normal_map_flat.png"
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
        onPressed: {
            console.log("ObjectPicker Picked!");
        }
    }

    Vector3dAnimation on position {
        from:Qt.vector3d(0,0,0); to:Qt.vector3d(0,0.5,0)
        duration: 5000
        easing.type: Easing.SineCurve
        loops: Animation.Infinite
        running: true
    }


    Scene2D {
        id: waylandScene
        output: RenderTargetOutput {
            id: waylandSceneRenderTargetOutput
            attachmentPoint: RenderTargetOutput.Color0
            texture: Texture2D {
                id: waylandSurfaceTexture
                height: shellSurf.surface.size.height - marginTop - marginBottom
                width: shellSurf.surface.size.width - marginLeft - marginRight
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
            height: waylandSurfaceTexture.height
            width: waylandSurfaceTexture.width


            ShellSurfaceItem {
                id:waylandQuickItem
                x:-marginLeft
                y:-marginTop
                height: shellSurf.surface.size.height
                width: shellSurf.surface.size.width
                shellSurface: shellSurf

                moveItem: Item {
                    visible: false
                }

                onSurfaceDestroyed: function() {
                    saveAppPrefs();
                    panel.destroy();
                    shellSurfaces.remove(index);
                }

                WaylandKeyboardHandler {
                    surf: shellSurf.surface

                    Component.onCompleted: {
                        physicalKeyboardAdapter.fullKeyEvent.connect(this.fullKeyEvent);
                    }
                }
            }
        }
    }

    Timer {
        onTriggered: function() {
            waylandScene.entities = [parent];
        }
        running: true
        interval: 250
        repeat: false
    }

    Launcher {
        id: processFinder

        Component.onCompleted: {
            processName = launch("ps -p "+shellSurf.surface.client.processId+" -o comm=");
            console.log(processName)
        }
    }

    function loadAppPref(name, fallback) {
        if(appPrefs.json[processName] && appPrefs.json[processName][name]) {
            return appPrefs.json[processName][name];
        } else if(appPrefs.json.global && appPrefs.json.global[name]) {
            return appPrefs.json.global[name];
        } else {
            return fallback;
        }
    }

    function saveAppPrefs() {
        appPrefs.json[processName] = {
            "ppm":ppm,
            "margin":margin,
            "marginTop":marginTop,
            "marginBottom":marginBottom,
            "marginLeft":marginLeft,
            "marginRight":marginRight
        }
        appPrefs.save();
    }
}
