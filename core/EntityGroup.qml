import Qt3D.Core 2.13
import Qt3D.Extras 2.13

Entity {
    property alias position: transform.translation
    property alias rotation: transform.rotation
    property alias rotationX: transform.rotationX
    property alias rotationY: transform.rotationY
    property alias rotationZ: transform.rotationZ
    property alias scale: transform.scale
    property alias scale3D: transform.scale3D
    property alias matrix: transform.matrix

    components: [
        Transform {
            id:transform
        }
    ]
}
