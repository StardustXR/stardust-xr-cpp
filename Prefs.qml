import QtQuick 2.0
import FileIO 1.0
import Launcher 1.0
import Qt.labs.platform 1.1

Launcher {
    id:prefsFile

    property variant jsonPrefs

    property string prefsFile
    property string defaultPrefsFile
    property string homePath: exec("echo ~")

    property FileIO fileWriter: FileIO {
        source: homePath+prefsFile

        Component.onCompleted: {
            if(!read()) {
                exec("mkdir -p ~/.config/stardust & touch "+source);
                console.log(source);

                var oldSource = source;
                source = defaultPrefsFile;

                jsonPrefs = JSON.parse(read());

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
        jsonPrefs = JSON.parse(appPrefsText);
    }

    function savePrefs() {
        fileWriter.write(JSON.stringify(jsonPrefs, null, "\t")+"\n");
    }
}
