import QtQuick 2.12
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtQuick.Window 2.12
import QtWayland.Compositor 1.3
import QtQuick.Scene3D 2.12

WaylandCompositor {
    // The output defines the screen.
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            width: 1024
            height: 768
            visible: true

            Scene3D {
                anchors.fill: parent
                anchors.margins: 0
                focus: true
                cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

                SceneRoot {
                    id:sceneRoot
                }
            }
        }
    }


    WlShell {
        onWlShellSurfaceCreated:
            shellSurfaces.append({shellSurface: shellSurface});
    }
    XdgShellV6 {
        onToplevelCreated:
            shellSurfaces.append({shellSurface: xdgSurface});
    }
    XdgShell {
        onToplevelCreated:
            shellSurfaces.append({shellSurface: xdgSurface});
    }
    IviApplication {
        onIviSurfaceCreated:
            shellSurfaces.append({shellSurface: iviSurface});
    }
    ListModel { id: shellSurfaces }
}
