import QtQuick 2.0
import QtQuick.Controls 2.12
import SortFilterProxyModel 0.2
import QtQuick.Layouts 1.12

import anmsettings 1.0

import "qrc:/src/qml/ethernet"
import "qrc:/src/qml/wifi"

Pane{
    id: rootItm


    signal notification(string title,string msg);

    ConnectionTreeInterface{
        id: tree;
    }

    DummyModel {
        id: test
    }

    EthernetTab{
        id: ethtab
        anchors.fill: parent
        visible: false
        z: 10
    }

    WifiTab{
        id: wifitab
        anchors.fill: parent
        visible: false
        z: 10
    }


    Component {
        id: sectionHeading


        Item{
            id: rect

            width: list.width
            height: 30
            anchors.margins: 0
            RowLayout{
                anchors.fill: parent
                anchors.margins: 0
                Layout.margins: 0
                spacing: 0
                Label {
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignVCenter
                    id: secLab
                    text: section
                    font.bold: true
                    font.pixelSize: 14
                    Layout.fillWidth: true
                }


                Button{
                    Layout.fillHeight: true
                    Layout.preferredWidth: 35
                    Layout.alignment: Qt.AlignVCenter
                    text: "+"
                    onClicked: {
                        if(section === "Ethernet"){
                            ethtab.visible = true;
                        } else if(section === "Wifi"){
                            wifitab.visible = true;
                        } else if(section === "Hotspot"){
                            wifitab.visible = true;
                        }



                    }
                }
            }
        }
    }


    ListView{
        id: list
        anchors.top: parent.top
        anchors.bottom: showall.top
        anchors.left: parent.left
        anchors.right: parent.right
        clip: true

         model: SortFilterProxyModel{
            sourceModel: tree.dataList
            filters:[
                RegExpFilter{
                    enabled: showall.checked
                    roleName: "nmPath"
                    pattern: "[A-Za-z0-9,-_.\s]+"
                    caseSensitivity: Qt.CaseInsensitive
                }
                ,
                ValueFilter{
                    enabled: !showall.checked
                    roleName: "available"
                    value: 1
                },
                AnyOf{
                    RegExpFilter {
                        enabled: vpnshow.checked
                        roleName: "groupe"
                        pattern: "VPN"
                        caseSensitivity: Qt.CaseInsensitive
                    }
                    RegExpFilter {
                        enabled: apshow.checked
                        roleName: "groupe"
                        pattern: "HOTSPOT"
                        caseSensitivity: Qt.CaseInsensitive
                    }
                    RegExpFilter {
                        enabled: wifishow.checked
                        roleName: "groupe"
                        pattern: "WIFI"
                        caseSensitivity: Qt.CaseInsensitive
                    }
                    RegExpFilter {
                        enabled: ethshow.checked
                        roleName: "groupe"
                        pattern: "ETHERNET"
                        caseSensitivity: Qt.CaseInsensitive
                    }
                }


            ]


            sorters: StringSorter { roleName: "groupe" }
        }
        delegate: ConnectionRowAdvanced{
            name_: name
            active_: available
            type_: 1
            signals_: signalStrength
            groupe_: groupe
            connected_: connected
            nmPath_: nmPath

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: parent.width/16

            height: 30

            onActivate: pwDialog.visible = true
            onNotification: {
                rootItm.notification(title,msg);
            }
        }

        section.delegate: sectionHeading
        section.property: "groupe"
        section.criteria: ViewSection.FullString

        boundsBehavior: Flickable.StopAtBounds
        ScrollIndicator.vertical: ScrollIndicator {
            id: scroller
            active: true
            onActiveChanged: {
                if(active !== true)
                {
                    active = true;
                }
            }
        }

    }

    Button {
        id: addbutton
        text: "+"
        onClicked: menu.open()
        anchors.top: showall.top
        anchors.left: parent.left
        anchors.bottom: showall.bottom
        Menu {
            id: menu
            title: "+"
            MenuItem {
                text: "+ ETHERNET"
                onClicked: {
                    ethtab.visible = true;
                }
            }
            MenuItem {
                text: "+ WIFI"
                onClicked: {
                    wifitab.visible = true;
                }
            }
        }
    }
    CheckBox{
        id: showall
        anchors.bottom: parent.bottom
        anchors.left: addbutton.right
        text: "ADVANCED"
    }
    CheckBox{
        anchors.right: wifishow.left
        anchors.bottom: parent.bottom
        id: ethshow
        checked: true
        text: "ETHERNET"
    }

    CheckBox{
        anchors.right: apshow.left
        anchors.bottom: parent.bottom
        id: wifishow
        checked: true
        text: "WIFI"
    }

    CheckBox{
        anchors.right: vpnshow.left
        anchors.bottom: parent.bottom
        id: apshow
        checked: true
        text: "HOTSPOT"
    }

    CheckBox{
        anchors.right: blueshow.left
        anchors.bottom: parent.bottom
        id: vpnshow
        checked: false
        text: "VPN"
    }

    CheckBox{
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        id: blueshow
        checked: false
        text: "BLUETOOTH"
    }


    PasswordDialog{
        id: pwDialog
        width: parent.width
        anchors.centerIn: parent
    }




}


