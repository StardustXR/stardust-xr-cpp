import QtQuick 2.14
import QtQuick3D 1.0

DefaultMaterial {
    property alias color: emissiveColor

    lighting: DefaultMaterial.NoLighting

    emissiveColor: Qt.rgba(1, 1, 1, 1)
}
