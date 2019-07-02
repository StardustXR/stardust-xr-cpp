import QtQuick 2.0
import QtWayland.Compositor 1.3
import QtQuick.Controls 2.5

import Launcher 1.0
import WaylandKeyboardHandler 1.0

Panel {

    property ShellSurface shellSurf
    property real listIndex
    property string processName

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
