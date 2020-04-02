import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12


//Do not import custom stuff here

// Load our plugin from filesystem
import "qrc:/"

ApplicationWindow {
    id: mainWindow
    visible: true
    title: "playground"

    width: 400
    height: 600

    // Add your Playground here

    /* Create an ApplicationWindow Item
    *  then add a loader and checkbox. Load your start window
    *  if the checkbox is chekced.Unload it when unchecked. Loading the Files with the checkbox will prevent
    *  the playground to crash because of other projects.
    */



    Loader {
        id: netLoader
        anchors.left: mainWindow.right
        anchors.top: mainWindow.top
    } //Networksettings Loader


    Loader {
        id: compLoader
        anchors.left: mainWindow.right
        anchors.top: mainWindow.top
    } //Networksettings Loader


    //Next Loader here

    GridLayout{
        anchors.fill: parent

        //Networkcomponents checkbox
        CheckBox{
            id: showNetComp
            checked: false;
            Layout.fillWidth: true;
            text: "Load netComp"
            Component.onCompleted: {
                if(checked){
                    netLoader.active=true;
                    netLoader.source = "Networksettings.qml";
                }else{
                    netLoader.active=false;
                }
            }
            onCheckStateChanged: {
                if(checked){
                    netLoader.active=true;
                    netLoader.source = "Networksettings.qml";
                }else{
                    netLoader.active=false;
                }
            }
        }


        CheckBox{
            id: showzeracomps
            checked: false;
            Layout.fillWidth: true;
            text: "Load zeraComp"
            Component.onCompleted: {
                if(checked){
                    compLoader.active=true;
                    compLoader.source = "zeracomps.qml";
                }else{
                    compLoader.active=false;
                }
            }
            onCheckStateChanged: {
                if(checked){
                    compLoader.active=true;
                    compLoader.source = "zeracomps.qml";
                }else{
                    compLoader.active=false;
                }
            }
        }


        // Next CheckBox here

    }

}


