import QtQuick 2.12
import QtQuick3D 1.0

Model {
    property real radius: 1
    scale: Qt.vector3d(0.01, 0.01, 0.01)
    onRadiusChanged: scale = Qt.vector3d(radius*0.01, radius*0.01, radius*0.01)

    source: "#Sphere"

    materials: [
        DefaultMaterial {
            lighting: DefaultMaterial.NoLighting

            diffuseMap: Texture {
                source: "qrc:/test_grid.png"
            }
        }

    ]
}
