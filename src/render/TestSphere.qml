import QtQuick3D 1.0

Model {
    property real radius: 1
    onRadiusChanged: scale = Qt.vector3d(radius, radius, radius)

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
