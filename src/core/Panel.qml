import QtQml 2.12
import QtQuick 2.12
import QtQuick3D 1.0

Model {
    id:panel

    property Item panelContents: Item{}

    property real ppm: 250

    property int margin: 0
    property int marginTop: margin
    property int marginBottom: margin
    property int marginLeft: margin
    property int marginRight: margin

    onMarginTopChanged: panelContents.y = -marginTop
    onMarginLeftChanged: panelContents.x = -marginLeft

    QtObject {
        id: props
        property size physicalDimensions: Qt.size(0.4, 0.4)
        property size pixelDimensions: Qt.size(100, 100)
        property size croppedDimensions: Qt.size(pixelDimensions.width-marginLeft-marginRight, pixelDimensions.height-marginTop-marginBottom)
    }

//    function setPhysicalDimensions(size) {
//        props.physicalDimensions = size;

//        props.pixelDimensions.x = size.x*ppm;
//        props.pixelDimensions.y = size.y*ppm;

//        panelContents.width = size.x*ppm;
//        panelContents.height = size.y*ppm;
//    }
//    function setPixelDimensions(size) {
//        props.pixelDimensions = size;

//        props.physicalDimensions.x = size.x/ppm;
//        props.physicalDimensions.y = size.y/ppm;

//        panelContents.width = size.x;
//        panelContents.height = size.y;
//    }

//    Connections {
//        target: panelContents

//        onWidthChanged: {props.pixelDimensions.x = panelContents.width}
//        onHeightChanged: {props.pixelDimensions.x = panelContents.height}
//    }

//    Component.onCompleted: {
//        setPixelDimensions(props.pixelDimensions);
//    }

    source: "#Rectangle"

//    scale: Qt.vector3d(props.croppedDimensions.width/ppm, props.croppedDimensions.height/ppm, 1)

    materials: DefaultMaterial {
        lighting: DefaultMaterial.NoLighting

        diffuseMap: Texture {
            sourceItem: Rectangle {
//                height: props.pixelDimensions.height
//                width: props.pixelDimensions.width
                color: "transparent"
                height: 100
                width: 100
            }
        }
    }
}
