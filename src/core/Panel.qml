import QtQuick 2.6
import QtQuick3D 1.0

import PassthroughKeyboardHandler 1.0

Model {
    property Item panelContents

    property real ppm: 250

    property real margin: 0
    property real marginTop: margin
    property real marginBottom: margin
    property real marginLeft: margin
    property real marginRight: margin

    property size dimensions: Qt.size(100, 100)
    property size croppedDimensions: Qt.size(dimensions.width-marginLeft-marginRight, dimensions.height-marginTop-marginBottom)

    id:panel

    source: "qrc:/Plane.ply"

    scale: Qt.vector3d(croppedDimensions.width/ppm, 1, croppedDimensions.height/ppm)

    materials: DefaultMaterial {
        diffuseMap: panelContents
    }
}
