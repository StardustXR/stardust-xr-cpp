import QtQuick 2.0
import FileIO 1.0
import Launcher 1.0
import Qt.labs.platform 1.1

Launcher {
    id:prefsFile

    property variant jsonPrefs

    property string prefsFilePath
    property string defaultPrefsFilePath
    property string configFolderPath: exec("echo $STARDUST_CONFIG_DIR")+"/"

    property FileIO fileWriter: FileIO {
        source: configFolderPath+prefsFilePath
    }

    Component.onCompleted: {
        if(configFolderPath == "/") {
            configFolderPath = exec("echo \"$HOME/.config/stardust\"")+"/";
            console.log("$STARDUST_CONFIG_DIR environment variable not set... defaulting to "+configFolderPath);
        }

        console.log("Attempting to read "+fileWriter.source);

        if(!fileWriter.read()) {
            exec("mkdir -p "+configFolderPath+" & touch "+configFolderPath+prefsFilePath);


            loadPrefs(defaultPrefsFilePath);

            savePrefs();
        } else {
            reloadPrefs();
        }
    }

    function exec(command) {
        return launch("sh -c \"" + command + "\"")

        //return launch(command);
    }

    function reloadPrefs() {
        loadPrefs(configFolderPath+prefsFilePath);
    }

    function loadPrefs(path) {
        fileWriter.source = path;
        var appPrefsText = fileWriter.read() || "{}";

        jsonPrefs = JSON.parse(appPrefsText);
        console.log(fileWriter.source);
        console.log(appPrefsText);
    }

    function savePrefs() {
        fileWriter.write(JSON.stringify(jsonPrefs, null, "\t")+"\n");
    }
}
