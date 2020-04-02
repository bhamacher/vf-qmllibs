import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
import networksettings 1.0

Pane{
    id: rootItm
    property WiredWrapper backend
    readonly property real tfratio: 1.9
    // Layout.minimumHeight: contentHeight
    // Layout.minimumWidth: contentWidth
    property int fontPixelSize
    padding: 0

    GridLayout{
        anchors.fill: parent
        anchors.margins: 0
        columns: 2

        RowLayout{
            Layout.margins: 0
            Layout.columnSpan: 2

            Label {
                id: ipv4Topic
                font.pixelSize: rootItm.fontPixelSize
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                text: qsTr("IPV4")
            }

            IpDetType{
                id: detT
                Layout.fillWidth: true
                backend: rootItm.backend
                fontPixelSize: rootItm.fontPixelSize
                ipType: "ipv4"
            }

        }


        Grid{
            id: indentation
            columns: 2
            Layout.fillWidth: true

            Item{
                id: spacer
                width: parent.width/8
                height:1
            }

            GridLayout{
                width: parent.width-spacer.width-10
                columnSpacing :2
                rowSpacing :2
                Layout.margins: 0
                columns:2

                Label {
                    id: ipv4Label
                    text: "IP  "
                    font.pixelSize: rootItm.fontPixelSize
                    verticalAlignment: Text.AlignVCenter
                }

                TextField {
                    id: ipv4
                    horizontalAlignment : TextInput.AlignRight
                    validator: RegExpValidator { regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/}
                    readOnly: {
                        if(WiredWrapper.Manual===backend.currentIpv4ConType){
                            return false;
                        }else{
                            return true;
                        }
                    }
                    Layout.fillWidth: true
                    //Layout.fillHeight: true
                    font.pixelSize: rootItm.fontPixelSize
                    text: {
                        if(focus && !readOnly){
                            return "";
                        }else{
                            return backend.ipv4;

                        }
                    }
                    onEditingFinished: {
                        rootItm.backend.ipv4 = text;
                    }
                }
                Label {
                    id: subnetmaskLabel
                    text: "SUBNETMASK  "
                    font.pixelSize: rootItm.fontPixelSize
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillHeight: true

                }
                TextField {
                    id: subnetmask
                    horizontalAlignment : TextInput.AlignRight
                    validator: RegExpValidator { regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/}
                    readOnly: {
                        if(WiredWrapper.Manual===backend.currentIpv4ConType){
                            return false;
                        }else{
                            return true;
                        }
                    }

                    font.pixelSize: rootItm.fontPixelSize
                    Layout.fillWidth: true
                    text: {
                        if(focus && !readOnly){
                            return "";
                        }else{
                            return backend.subnetmask;

                        }
                    }
                    onEditingFinished: {
                        rootItm.backend.subnetmask = text;
                    }

                }


            }
        }
    }
}

