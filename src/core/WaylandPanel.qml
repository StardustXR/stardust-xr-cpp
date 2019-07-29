import QtQuick 2.0
import QtWayland.Compositor 1.3
import QtQuick.Controls 2.5

import Qt3D.Core 2.12
import Qt3D.Extras 2.12

import Launcher 1.0
import WaylandKeyboardHandler 1.0

EntityGroup {
    id:panel

    property alias shellSurf: waylandPanel.shellSurf
    property alias listIndex: waylandPanel.listIndex
    property alias processName: waylandPanel.processName
    property alias ppm: waylandPanel.ppm
    property alias dimensions: waylandPanel.dimensions

    property real headerHeight: 0.03
    property string titleText: "Title"

    Panel {
        id:waylandPanel

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

        panelContents: waylandQuickItem

        ShellSurfaceItem {
            id:waylandQuickItem

            x:-parent.marginLeft
            y:-parent.marginTop
            height: shellSurf.surface.size.height
            width: shellSurf.surface.size.width

            shellSurface: shellSurf

            moveItem: Item {
                visible: false
            }

            Component.onCompleted: {
                var panelSize = waylandPanel.loadAppPref("defaultSize", undefined);
                if(panelSize !== undefined) {
                    if(shellSurface instanceof WlShellSurface) {
                        shellSurface.sendConfigure(panelSize, 0);
                    } else if (shellSurface instanceof XdgSurface) {
                        shellSurface.toplevel.sendUnmaximized(panelSize);
                    } else if (shellSurface instanceof IviSurface) {
                        shellSurface.sendConfigure(panelSize);
                    }
                }
            }

            onSurfaceDestroyed: panel.close()

//            WaylandKeyboardHandler {
//                surf: shellSurf.surface

//                Component.onCompleted: {
//                    physicalKeyboardAdapter.fullKeyEvent.connect(this.fullKeyEvent);
//                }
//            }
        }

        Launcher {
            id: processFinder

            Component.onCompleted: {
                processName = launch("ps -p "+shellSurf.surface.client.processId+" -o comm=");

                waylandPanel.saveAppPrefs();

                titleText = shellSurf.title || processName;
                console.log(titleText);
            }
        }

        function loadAppPref(name, fallback) {
            var value;
            if(appPrefs.json[processName] && appPrefs.json[processName][name]) {
                value = appPrefs.json[processName][name];
            } else if(appPrefs.json.global && appPrefs.json.global[name]) {
                value = appPrefs.json.global[name];
            } else {
                value = fallback;
            }

            if(value && value.height && value.width) {
                value = Qt.size(value.width, value.height);
            }

//            console.log(name+": "+value);
            return value;
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

    Panel {
        id:defaultPanelHeader

        property real unitHeight: panel.headerHeight

        ppm:waylandPanel.ppm

        dimensions: Qt.size(waylandPanel.croppedDimensions.width, unitHeight*ppm)
        position: Qt.vector3d(0,0,waylandPanel.croppedDimensions.height/2/ppm+unitHeight/2)
        panelContents:titleContent

        DesktopPanelTitleContent {
            id:titleContent
            width: defaultPanelHeader.dimensions.width
            height: defaultPanelHeader.dimensions.height
            ppm:panel.ppm

            titleText: panel.titleText
        }
    }

    function close() {
        waylandQuickItem.bufferLocked = true;
        shellSurfaces.remove(index);
    }
}
