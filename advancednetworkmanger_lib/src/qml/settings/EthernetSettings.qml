import QtQuick 2.0

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

    WiredConnectionSettingsInterface{
        id: backend
        onLoadComplete: {
            name.text = backend.conName;
            ipv4Mode


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
                }


        }


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

                Label{
                    id: ipv4ModeLabel
                    text: "MODE"
                    Layout.preferredWidth: clientModel.labelWidth
                }
                ComboBox{
                    id: ipv4Mode
                    Layout.fillWidth: true
                    model: ["DHCP", "MANUAL"]
                }
            }


            RowLayout{
                id: ipv4ip
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 20

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
                }
            }


            RowLayout{
                id: ipv4Sub
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 20

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

                Label{
                    id: ipv6ModeLabel
                    text: "MODE"
                    Layout.preferredWidth: clientModel.labelWidth
                }
                ComboBox{
                    id: ipv6Mode
                    Layout.fillWidth: true
                    model: ["DHCP", "MANUAL"]
                }
            }



            RowLayout{
                id: ipv6ip
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 20

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
                }
            }


            RowLayout{
                id: ipv6Sub
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 20

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
        id: abort
        text: "ABORT"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 0
        onClicked: {
            rootItm.visible = false
        }
    }

    Button{
        id: save
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        text: "SAVE"
    }

    AvailableApDialog{
        id: aApDialog
        width: parent.width
        anchors.centerIn: parent
    }

}









