import QtQuick 2.0
import Qt3D.Core 2.12
import Qt3D.Extras 2.12

ForwardRenderer {
    id:renderer
    property Entity monoCam
//    property Entity stereoCam

    clearColor: Qt.rgba(0, 0, 0, 1)
    camera: monoCam
}
