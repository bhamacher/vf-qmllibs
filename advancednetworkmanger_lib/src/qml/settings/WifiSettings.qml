import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import SortFilterProxyModel 0.2
import anmsettings 1.0
import QtQml.Models 2.11
import QtQuick.Controls.Material 2.12

import "qrc:/src/qml/FontAwesome.js" as FA

Pane{
    id: rootItm
    padding: 0
    property string path : ""

    signal notification(string title,string msg);

    function init() {
        if(path === ""){
            backend.create();
        }else{
            backend.load(path);
        }
    }

    NetworkmanagerAbstraction{
        id: generalbackend
    }


    WirelessConnectionSettingsInterface{
        id: backend
        onLoadComplete: {
            name.text = backend.conName;
            ssid.text = backend.ssid;
            pw.text = backend.password;
            if(backend.mode === "CLIENT"){
                mode.currentIndex = 0
            }else if(backend.mode === "HOTSPOT"){
                mode.currentIndex = 1
            }

        }
    }


    ObjectModel{
        id: clientModel
        property int labelWidth : rootItm.width/4

        Label{
            id: header
            anchors.left: parent.left
            anchors.right: parent.right
            font.bold: true
            font.pixelSize: 18
            font.underline: true
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
                validator: RegExpValidator{ regExp: /.{3,}/}
                Material.accent:  {
                    if(!acceptableInput){
                        return Material.Red;
                    }else{
                        return Material.Green;
                    }
                }
                onEditingFinished: {
                    backend.conName = text;
                }
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
                validator: RegExpValidator{ regExp: /.{1,}/}
                Material.accent:  {
                    if(!acceptableInput){
                        return Material.Red;
                    }else{
                        return Material.Green;
                    }
                }
                onEditingFinished: {
                    backend.ssid = text;
                }
            }
            Button{
                text: FA.icon(FA.fa_search_plus,null);
                background: Rectangle{
                    color: "transparent"
                }
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
                id: pw
                echoMode: TextInput.Password
                Layout.fillWidth: true
                validator: RegExpValidator{ regExp: /.{8,}/}
                Material.accent:  {
                    if(!acceptableInput){
                        return Material.Red;
                    }else{
                        return Material.Green;
                    }
                }
                onEditingFinished: {
                    backend.password = text;
                }
            }
            Button{
                id: pwvisible
                text: FA.icon(FA.fa_eye_slash,null)
                font.pixelSize: rootItm.fontPixelSize
                background: Rectangle{
                    color: "transparent"
                }
                onPressed: {
                    pw.echoMode = TextInput.Normal
                    pwvisible.text= FA.icon(FA.fa_eye,null)
                }
                onReleased: {
                    pw.echoMode = TextInput.Password
                    pwvisible.text= FA.icon(FA.fa_eye_slash,null)
                }
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
                onCurrentIndexChanged: {
                    if(currentIndex<count && currentIndex>=0){
                        backend.mode=model[currentIndex];
                    }
                }
            }
        }




    }




    ListView{
        id: list
        anchors.top: parent.top
        anchors.bottom: save.top
        anchors.left: parent.left
        anchors.right: parent.right
        model: clientModel
    }


    Button{
        id: abort
        text: "CANCEL"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        onClicked: {
            backend.discard();
            rootItm.visible = false

        }
    }

    Button{
        id: save
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: "SAVE"
        onClicked: {
            var good=true;
            var errorField="";
            if(!name.acceptableInput){
                good = false;
                errorField="NAME"
            }else if(!ssid.acceptableInput){
                good = false;
                errorField="SSID"
            }else if(!pw.acceptableInput){
                good = false;
                errorField="PASSWORD"
            }
            if(good){
                backend.save();
                rootItm.visible = false
            }else{
                notification("NM", "invalid settings in field: " + errorField)
            }
        }
    }

    AvailableApDialog{
        id: aApDialog
        width: parent.width*0.9
        anchors.centerIn: parent
        onOkPressed: {
            ssid.text = retSsid;
            backend.ssid = retSsid;
        }
    }

}









