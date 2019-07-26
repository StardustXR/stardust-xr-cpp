import QtQuick 2.0
import ExtensionLoader 1.0

ExtensionLoader {
    id:extensionLoader
    property list<Component> extensions

    Component.onCompleted: {
        extensions = extensionsList.map(x => Qt.createComponent("file://"+x));
    }

    function loadExtension(extension, parent, attributes, onFinished) {
        var extensionInstance = extension.incubateObject(parent, attributes);
        if (extensionInstance.status !== Component.Ready) {
            extensionInstance.onStatusChanged = function(status) {
                if (status === Component.Ready) {
                    onFinished(extensionInstance);
                }
            }
        } else {
            onFinished(extensionInstance);
        }
    }
}
