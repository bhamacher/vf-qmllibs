import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import anmsettings 1.0

import "qrc:/src/qml/FontAwesome.js" as FA

Pane{
    id: rootItm
    padding: 0
    property string path : ""


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

    WiredConnectionSettingsInterface{
        id: backend
        onLoadComplete: {
            name.text = backend.conName;
            if(backend.ipv4Mode === "DHCP"){
                ipv4Mode.currentIndex = 0
            }else if(backend.ipv4Mode === "MANUAL"){
                ipv4Mode.currentIndex = 1
            }

            ipv4.text = backend.ipv4;
            sub4.text = backend.ipv4Sub;
            ipv6.text = backend.ipv6;
            sub6.text = backend.ipv6Sub;
            var index = devices.find(backend.device,Qt.MatchExactly);
            if(index >= 0 ){
                devices.currentIndex = index;
            }else{
                devices.currentIndex = 0;
            }
            backend.device = devices.model[devices.currentIndex]

            if(backend.ipv6Mode === "DHCP"){
                ipv6Mode.currentIndex = 0
            }else if(backend.ipv6Mode === "MANUAL"){
                ipv6Mode.currentIndex = 1
            }


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
            font.pixelSize: 18
            font.underline: true
            horizontalAlignment: Label.AlignHCenter

            text: "ETHERNET CONNECTION SETTINGS"
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
                onEditingFinished: {
                    backend.conName = text;
                }
            }


        }

//        RowLayout{
//            id: deviceName
//            anchors.left: parent.left
//            anchors.right: parent.right

//            Label{
//                id: deviceLabel
//                text: "DEVICE"
//                Layout.preferredWidth: clientModel.labelWidth
//            }

//            ComboBox{
//                id: devices
//                Layout.fillWidth: true
//                model: backend.devices
//                onCurrentIndexChanged: {
//                    backend.device = model[currentIndex]
//                }
//            }

//        }



        Label{
            id: ipv4header
            anchors.left: parent.left
            anchors.right: parent.right
            font.bold: true
            text: "IPV4"
        }


        RowLayout{
            id: ipv4ModeL
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            spacing: 0
            Label{
                id: ipv4ModeLabel
                text: "MODE"
                Layout.preferredWidth: clientModel.labelWidth
            }
            ComboBox{
                id: ipv4Mode
                Layout.fillWidth: true
                model: ["DHCP", "MANUAL"]
                onCurrentIndexChanged: {
                    backend.ipv4Mode = model[currentIndex]
                }
            }
        }


        RowLayout{
            id: ipv4ip
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            spacing: 0
            Label{
                id: ipv4ipLabel
                text: "IP"
                Layout.preferredWidth: clientModel.labelWidth
            }
            TextField {
                id: ipv4
                horizontalAlignment : TextInput.AlignRight
                validator: RegExpValidator { regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/}
                Layout.fillWidth: true
                onEditingFinished: {
                    backend.ipv4 = text;
                }
                enabled: {
                    if(ipv4Mode.currentText === "DHCP"){
                        return false;
                    }else{
                        return true;
                    }
                }
            }
        }


        RowLayout{
            id: ipv4Sub
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            spacing: 0
            Label{
                id: ipv4SubLabel
                text: "SUBNETMASK"
                Layout.preferredWidth: clientModel.labelWidth
            }
            TextField {
                id: sub4
                horizontalAlignment : TextInput.AlignRight
                validator: RegExpValidator { regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/}
                Layout.fillWidth: true
                enabled: {
                    if(ipv4Mode.currentText === "DHCP"){
                        return false;
                    }else{
                        return true;
                    }
                }
                onEditingFinished: {
                    backend.ipv4Sub = text;
                }

            }
        }

        Label{
            id: ipv6header
            anchors.left: parent.left
            anchors.right: parent.right
            font.bold: true
            text: "IPV6"
        }

        RowLayout{
            id: ipv6ModeL
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            spacing: 0
            Label{
                id: ipv6ModeLabel
                text: "MODE"
                Layout.preferredWidth: clientModel.labelWidth
            }
            ComboBox{
                id: ipv6Mode
                Layout.fillWidth: true
                model: ["DHCP", "MANUAL"]
                onCurrentIndexChanged: {
                    backend.ipv6Mode = model[currentIndex]
                }
            }
        }



        RowLayout{
            id: ipv6ip
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            spacing: 0
            Label{
                id: ipv6ipLabel
                text: "IP"
                Layout.preferredWidth: clientModel.labelWidth
            }
            TextField {
                id: ipv6
                horizontalAlignment : TextInput.AlignRight
                validator: RegExpValidator { regExp: /([a-f0-9:]+:+)+[a-f0-9]+/}
                Layout.fillWidth: true
                enabled: {
                    if(ipv6Mode.currentText === "DHCP"){
                        return false;
                    }else{
                        return true;
                    }
                }
            }
        }


        RowLayout{
            id: ipv6Sub
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            spacing: 0
            Label{
                id: ipv6SubLabel
                text: "SUBNETMASK"
                Layout.preferredWidth: clientModel.labelWidth
            }
            TextField {
                id: sub6
                horizontalAlignment : TextInput.AlignRight
                validator: RegExpValidator { regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/}
                Layout.fillWidth: true
                enabled: {
                    if(ipv6Mode.currentText === "DHCP"){
                        return false;
                    }else{
                        return true;
                    }
                }
                onEditingFinished: {
                    backend.ipv6Sub = text;
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
        model:clientModel
        clip: true
    }


    Button{
        id: cancel
        text: "CANCEL"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 0
        onClicked: {
            backend.discard();
            rootItm.visible = false
        }
    }

    Button{
        id: save
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        text: "SAVE"
        onClicked: {
            var good = true;
            if(good){
                backend.save();
                rootItm.visible = false
            }
        }
    }

    AvailableApDialog{
        id: aApDialog
        width: parent.width
        anchors.centerIn: parent
    }

}









