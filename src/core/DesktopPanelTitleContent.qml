import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Rectangle {
    id:title
    property real ppm
    property string titleText
    property real margin: 4

    color: Qt.rgba(0, 0.5, 1, 0.5)
    border.color: Qt.rgba(0,1,2,1)
    border.width: 1

    RowLayout {
        anchors.fill: parent
        Rectangle {
            Layout.minimumHeight: margin
        }

        Label {
            id:titleTextLabel
            Layout.maximumHeight: parent.height

            Layout.minimumWidth: parent.width/2
            Layout.fillWidth: true

            font.family: "Inter"
            font.bold: true
            font.pixelSize: parent.height-(margin*2)
            antialiasing: false

            color: "white"
            text: titleText
        }

        Button {
            Layout.maximumHeight: parent.height
            Layout.minimumWidth: parent.height
            Layout.preferredWidth: parent.height

            flat:true

            text: qsTr("✖")
            onClicked: panel.close()

            background: Rectangle {
                color: "red"
            }
        }
    }
}
