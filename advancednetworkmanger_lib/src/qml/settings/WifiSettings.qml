import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import anmsettings 1.0

import "qrc:/src/qml/network"

import "qrc:/src/qml/FontAwesome.js" as FA

Pane{
    id: rootItm
    padding: 0
    property string path: ""


    onPathChanged: {
        backend.load(path);
    }

    WirelessConnectionSettingsInterface{
        id: backend
        onLoadComplete: {
            name.text = backend.conName;
        }
    }


    VisualItemModel{
        id: clientModel
        property int labelWidth : rootItm.width/4

        Label{
            id: header
            anchors.left: parent.left
            anchors.right: parent.right
            font.bold: true
            horizontalAlignment: Label.AlignHCenter

            text: "WIFI CONNECTION SETTINGS"
        }



        RowLayout{
            id: conName
            anchors.left: parent.left
            anchors.right: parent.right


            Label{
                id: nameLabel
                text: "CONNECTION NAME"
                Layout.preferredWidth: clientModel.labelWidth
            }

            TextField{
                id: name
                Layout.fillWidth: true
            }


        }


        RowLayout{
            id: conSsid
            anchors.left: parent.left
            anchors.right: parent.right

            Label{
                id: ssidLabel
                text: "SSID"
                Layout.preferredWidth: clientModel.labelWidth
            }
            TextField{
                id: ssid
                Layout.fillWidth: true
            }
            Button{
                text: FA.icon(FA.fa_search_plus,null);
                onClicked: {
                    aApDialog.visible = true;
                }
            }

        }

        RowLayout{
            id: conPassword
            anchors.left: parent.left
            anchors.right: parent.right

            Label{
                id: passwordLabel
                text: "PASSWORD"
                Layout.preferredWidth: clientModel.labelWidth
            }
            TextField{
                id: password
                echoMode: TextInput.Password
                Layout.fillWidth: true
            }
        }

        RowLayout{
            id: conMode
            anchors.left: parent.left
            anchors.right: parent.right

            Label{
                id: modeLabel
                text: "MODE"
                Layout.preferredWidth: clientModel.labelWidth
            }
            ComboBox{
                id: mode
                Layout.fillWidth: true
                model: ["CLIENT", "HOTSPOT"]
            }
        }




    }




    ListView{
        id: list
        anchors.top: parent.top
        anchors.bottom: save.top
        anchors.left: parent.left
        anchors.right: parent.right
        model:clientModel
    }


    Button{
        id: abort
        text: "ABORT"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        onClicked: {
            rootItm.visible = false
        }
    }

    Button{
        id: save
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: "SAVE"
    }

    AvailableApDialog{
        id: aApDialog
        width: parent.width
        anchors.centerIn: parent
    }

}









