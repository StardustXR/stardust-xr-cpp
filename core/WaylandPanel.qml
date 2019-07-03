import QtQuick 2.0
import QtWayland.Compositor 1.3
import QtQuick.Controls 2.5

import Qt3D.Core 2.12
import Qt3D.Extras 2.12

import Launcher 1.0
import WaylandKeyboardHandler 1.0

Panel {
    property ShellSurface shellSurf
    property real listIndex
    property string processName

    ppm: loadAppPref("ppm", 1000)

    margin: loadAppPref("margin", 0)
    marginTop: loadAppPref("marginTop", margin)
    marginBottom: loadAppPref("marginBottom", margin)
    marginLeft: loadAppPref("marginLeft", margin)
    marginRight: loadAppPref("marginRight", margin)

    dimensions: shellSurf.surface.size

    panelContents: ShellSurfaceItem {
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

    Vector3dAnimation on position {
        from:Qt.vector3d(0,0,0); to:Qt.vector3d(0,0.1,0)
        duration: 5000
        easing.type: Easing.SineCurve
        loops: Animation.Infinite
        running: true
    }

    Launcher {
        id: processFinder

        Component.onCompleted: {
            processName = launch("ps -p "+shellSurf.surface.client.processId+" -o comm=");
            console.log(processName);
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

    Panel {
        id:defaultPanelHeader
        ppm:parent.ppm

        dimensions: Qt.size(parent.dimensions.width, 0.02*ppm)
        panelContents: Rectangle {
            color: "white"
            anchors.fill: parent
        }
    }
}
