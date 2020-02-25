import QtQuick 2.12
import QtQuick3D 1.0

Model {
    property Item sourceItem

    source: "#Rectangle"

    materials: DefaultMaterial {
        lighting: DefaultMaterial.NoLighting

        diffuseMap: Texture {
            sourceItem: sourceItem;
        }
    }
}
