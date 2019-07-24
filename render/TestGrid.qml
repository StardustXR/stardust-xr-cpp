import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Extras 2.12

Entity {
    id:testGrid

    property real size: 1000

    components: [
        PlaneMesh {
            meshResolution: Qt.size(2,2)
            height: size
            width: size
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
                    source: "qrc:/res/test_grid.png"
                }
            }
        },
        Transform {
            translation: Qt.vector3d(0,0,-1);
        }

    ]
}
