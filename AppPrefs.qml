import QtQuick 2.0
import FileIO 1.0
import Launcher 1.0
import Qt.labs.platform 1.1

Launcher {
    id:appPrefsFile

    property variant jsonAppPrefs

    property string homePath: exec("echo ~")

    property FileIO fileWriter: FileIO {
        source: homePath+"/.config/stardust/app_prefs.json"

        Component.onCompleted: {
            if(!read()) {
                exec("mkdir -p ~/.config/stardust & touch "+source);
                console.log(source);

                var oldSource = source;
                source = ":/app_prefs.json";

                jsonAppPrefs = JSON.parse(read());

                source = oldSource;

                savePrefs();
            }
        }
    }

    function exec(command) {
        return launch("sh -c \"" + command + "\"")

        //return launch(command);
    }

    Component.onCompleted: loadPrefs();

    function loadPrefs() {
        var appPrefsText = fileWriter.read() || "{}";

        console.log(appPrefsText);
        jsonAppPrefs = JSON.parse(appPrefsText);
    }

    function savePrefs() {
        fileWriter.write(JSON.stringify(jsonAppPrefs, null, "\t")+"\n");
    }
}
