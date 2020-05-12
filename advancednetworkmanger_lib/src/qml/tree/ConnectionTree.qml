import QtQuick 2.0
import QtQuick.Controls 2.12
import SortFilterProxyModel 0.2
import QtQuick.Layouts 1.12

import anmsettings 1.0

import "qrc:/src/qml/settings"

Pane{
    id: rootItm
    padding: 0

    signal notification(string title,string msg);

    ConnectionTreeInterface{
        id: backend;
    }





    Component{
        id: ethtab

        EthernetSettings{
            anchors.fill: parent
            anchors.margins: 0
            visible: true
            z: 10


            onVisibleChanged: {
                if(!visible){
                    ethLoader.active = false;
                }
            }

        }
    }

    Component{
        id: wifitab

        WifiSettings{

            anchors.fill: parent
            anchors.margins: 0
            visible: true
            z: 10

            onVisibleChanged: {
                if(!visible){
                    wifiLoader.active = false;
                }
            }

        }
    }

    Loader {
        id: ethLoader
        anchors.fill: parent
        anchors.margins: 0
        active : false
        z: 10
        sourceComponent: ethtab
        property string path: ""
        onLoaded: {
            item.path = ethLoader.path
        }
        onActiveChanged: {
            if(!active){
                path = "";
            }
        }
    }

    Loader{
        id: wifiLoader
        anchors.fill: parent
        anchors.margins: 0
        visible: true
        active : false
        z: 10
        sourceComponent: wifitab
        property string path: ""
        onLoaded: {
            item.path = wifiLoader.path
        }
        onActiveChanged: {
            if(!active){
                path = "";
            }
        }
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
            sourceModel: backend.dataList
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
            available_: available
            type_: type
            signals_: signalStrength
            groupe_: groupe
            connected_: connected
            nmPath_: nmPath
            stored_: stored

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: parent.width/30

            height: 30


            onEdit: {
                if(groupe_ == "ETHERNET"){
                    ethLoader.path = p_path;
                    ethLoader.active = true;
                }else if(groupe_ === "WIFI"){
                    wifiLoader.path = p_path;
                    wifiLoader.active = true;

                }else if(groupe_ === "HOTSPOT"){
                    wifiLoader.path = p_path;
                    wifiLoader.active = true;
                }
            }

            onRemove: {
                backend.removeConnection(p_path)
            }

            onActivate: {
                var Devices = backend.getDevices(type_)
                if(Devices.length > 1){
                    rootItm.notification("Devices", "more then 1:\n" + Devices[0]);
                }else if(Devices.length > 0){
                    rootItm.notification("Devices",Devices[0]);
                    backend.connect(p_path,Devices[0]);
                }
            }
            onDeactivate: {
                backend.disconnect(p_path)
            }

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

                    ethLoader.active = true;
                }
            }
            MenuItem {
                text: "+ WIFI"
                onClicked: {

                    wifiLoader.active = true;
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



