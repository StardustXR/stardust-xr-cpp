import QtQuick 2.14
import QtQuick3D 1.0

import StardustAPI.Fields 1.0

Model {
    property real radius: 1
    property alias field: sphereField
    property Material material: DefaultMaterial {
        lighting: DefaultMaterial.NoLighting
    }

    source: "#Sphere"
    materials: [material]

    scale: Qt.vector3d(radius*0.01, radius*0.01, radius*0.01)

    SphereField {
        id: sphereField

        radius: parent.radius*100
    }
}
