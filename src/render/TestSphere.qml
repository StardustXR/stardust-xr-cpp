import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Extras 2.12

Entity {
    id:testGrid

    property real size: 100
    property vector3d position: Qt.vector3d(0,-5,0)

    components: [
        SphereMesh {
            radius: size
            rings: 64
            slices: 128
        },
        TextureMaterial {
            texture: Texture2D {
                minificationFilter: Texture.Linear
                magnificationFilter: Texture.Linear
                wrapMode {
                    x: WrapMode.Repeat
                    y: WrapMode.Repeat
                }
                TextureImage {
                    source: "qrc:/test_grid.png"
                }
            }
        },
        Transform {
            translation: position
            rotationX: 90
            scale3D: Qt.vector3d(1,-1,1)
        }

    ]
}
