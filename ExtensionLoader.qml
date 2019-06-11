import QtQuick 2.0
import Launcher 1.0

QtObject {
    property variant extensions
    property string extensionType
    property Launcher launcher: Launcher {
        function exec(command) {
            return launch("sh -c \"" + command + "\"")
        }
    }

    Component.onCompleted: {
        launcher.exec("mkdir -p ~/.config/stardust/extensions");
        launcher.exec("mkdir -p ~/.config/stardust/extensions/"+extensionType);
        reload();
    }

    function reload() {
        var fileNames = launcher.exec("ls -d ~/.config/stardust/extensions/"+extensionType+"/*.qml").split("\n");
        extensions = fileNames.map(fileName => Qt.createComponent(fileName));
    }
}
