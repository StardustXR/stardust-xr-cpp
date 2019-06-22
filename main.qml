import QtQuick 2.12
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtQuick.Window 2.12
import QtWayland.Compositor 1.3
import QtQuick.Scene3D 2.12
import Preferences 1.0

WaylandCompositor {
    // The output defines the screen.
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            width: 1024
            height: 768
            visible: true

            Scene3D {
                aspects: [ "input", "render" ]
                anchors.fill: parent
                anchors.margins: 0
                focus: true
                cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
                hoverEnabled: true

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
