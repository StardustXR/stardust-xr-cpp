import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick3D 1.0
import QtQuick3D.MaterialLibrary 1.0
import QtWayland.Compositor 1.13

import Preferences 1.0

import Vulkan 1.0
import OpenXR 1.0
import OpenXRGraphics 1.0

WaylandCompositor {
    id:waylandCompositor
    useHardwareIntegrationExtension: false

    // The output defines the screen.
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            id: displaySurface
            width: 1600
            height: 900
            visible: true

            View3D {
                id: leftView

                renderMode: View3D.Texture

                x:0
                y:0
                height: OpenXRGraphics.eyeDimensions.height
                width: OpenXRGraphics.eyeDimensions.width

                camera: sceneRoot.leftEye
                scene: sceneRoot
                environment: sceneRoot.skybox
            }

            View3D {
                id: rightView

                renderMode: View3D.Texture

                x:OpenXRGraphics.eyeDimensions.width
                y:0
                height: OpenXRGraphics.eyeDimensions.height
                width: OpenXRGraphics.eyeDimensions.width

                camera: sceneRoot.rightEye
                scene: sceneRoot
                environment: sceneRoot.skybox
            }

            onSceneGraphInitialized: {
        OpenXR.vulkan = Vulkan;
        Vulkan.openxr = OpenXR;
        OpenXR.initialize();

                OpenXRGraphics.leftEye = sceneRoot.leftEye;
                OpenXRGraphics.rightEye = sceneRoot.rightEye;

                OpenXRGraphics.leftView = leftView;
                OpenXRGraphics.rightView = rightView;

                OpenXRGraphics.openxr = OpenXR;

                OpenXRGraphics.initialize();

                displaySurface.width = OpenXRGraphics.eyeDimensions.width;
                displaySurface.height = OpenXRGraphics.eyeDimensions.height;
            }
        }
    }

    SceneRoot {
        id:sceneRoot
    }

    WlShell {
        onWlShellSurfaceCreated:
            shellSurfaces.append({shellSurface: shellSurface});
    }
    IviApplication {
        onIviSurfaceCreated:
            shellSurfaces.append({shellSurface: iviSurface});
    }
    XdgShellV6 {
        onToplevelCreated:
            shellSurfaces.append({shellSurface: xdgSurface});
    }
    XdgShell {
        onToplevelCreated:
            shellSurfaces.append({shellSurface: xdgSurface});
    }
    XdgDecorationManagerV1 {
        preferredMode: XdgToplevel.ServerSideDecoration
    }

    ListModel { id: shellSurfaces }

    Preferences {
        id:appPrefs
        fileName: "app_prefs.json"
    }

    Preferences {
        id:compositorPrefs
        fileName: "compositor_prefs.json"
    }
}

//Window {
//    id: window
//    width: 640
//    height: 640
//    visible: true
//    color: "black"

//    View3D {
//        id: layer1
//        anchors.fill: parent
//        anchors.margins: 50
//        camera: scene.camera
//        renderMode: View3D.Overlay

//        environment: scene.skybox
//        scene: scene

//    }

//    Node {
//        id:scene
//        property alias skybox: skybox
//        property alias camera: camera

//        SceneEnvironment {
//            id:skybox
//            probeBrightness: 1000
//            backgroundMode: SceneEnvironment.Color
//            clearColor: "skyblue"
////            lightProbe: Texture {
////                source: "qrc:/test_grid.png"
////            }
//        }
//        Camera {
//            id: camera
//            position: Qt.vector3d(0, 200, -300)
//            rotation: Qt.vector3d(30, 0, 0)
//        }
//    }


//}
