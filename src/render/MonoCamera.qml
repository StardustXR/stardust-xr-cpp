import Qt3D.Core 2.13
import Qt3D.Render 2.13

Entity {
    id: root

    property vector3d position: Qt.vector3d(0.0, 0.0, 10.0)
    property vector3d viewVector: Qt.vector3d(0.0, 1.0, 0.0)
    property vector3d viewCenter: Qt.vector3d(0.0, 1.0, 0.0)
    property vector3d upVector: Qt.vector3d(0.0, 0.0, 1.0)
    property CameraLens lens: null

    components: [lens, transform]

    Transform {
        id: transform
        matrix: {
            var m = Qt.matrix4x4();
            m.translate(root.position)
            var yAxis = root.viewVector.times(-1).normalized();
//            var yAxis = viewCenter.minus(root.position).normalized();
            var xAxis = root.upVector.crossProduct(yAxis).normalized();
            var zAxis = yAxis.crossProduct(xAxis);
            var r = Qt.matrix4x4(xAxis.x, zAxis.x, yAxis.x, 0,
                                 xAxis.y, zAxis.y, yAxis.y, 0,
                                 xAxis.z, zAxis.z, yAxis.z, 0,
                                 0, 0, 0, 1);
            return m.times(r);
        }
    }
}
