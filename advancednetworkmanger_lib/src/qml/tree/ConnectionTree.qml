import QtQuick 2.0
import QtQuick.Controls 2.12
import SortFilterProxyModel 0.2
import QtQuick.Layouts 1.12

import anmsettings 1.0

import "qrc:/src/qml/settings"
import "qrc:/src/qml/FontAwesome.js" as FA

Pane{
    id: rootItm
    padding: 0

    signal notification(string title,string msg);



    ConnectionTreeInterface{
        id: backend;
    }



    FontLoader {
        source: "qrc:../3rdparty/font-awesome-4.6.1/fonts/fontawesome-webfont.ttf"
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


    Component{
        id: infotab
        ConnectionInfo{

            anchors.top: parent.top
            anchors.bottom: showall.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 0
            visible: true
            z: 10
        }
    }






    Component{
        id: pwDialog
        SmartConnect{
            width: parent.width
            anchors.centerIn: parent
            visible: true
            onVisibleChanged: {
                if(!visible){
                    smartConnectLoader.active = false;
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
            item.init();
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
            item.init();
        }
        onActiveChanged: {
            if(!active){
                path = "";
            }
        }
    }

    Loader{
        id: infoLoader
        anchors.top: parent.top
        anchors.bottom: showall.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 0
        visible: true
        active : false
        z: 10
        sourceComponent: infotab
    }

    Loader {
        id: smartConnectLoader
        width: parent.width*0.9
        anchors.centerIn: parent
        active : false
        visible: true
        sourceComponent: pwDialog
        property string ssid: ""
        property string device: ""
        onLoaded: {
            item.init(ssid,device)
        }
    }




    Component {
        id: sectionHeading


        Item{
            id: rect
            width: list.width
            height: 15
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
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 35
                    Layout.alignment: Qt.AlignVCenter
                    text: "+"
                    onClicked: {
                        if(section === "ETHERNET"){
                            ethLoader.active = true;
                        } else if(section === "WIFI"){
                            wifiLoader.active = true;
                        } else if(section === "HOTSPOT"){
                            wifiLoader.active = true;
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
                //                RangeFilter{
                //                  enabled: !showall.checked
                //                  roleName: "signalStrength"
                //                  minimumValue: 25
                //                },
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
                var Device;
                //                if(Devices.length>1){
                //                    if(stored){
                //                        deviceDialogLoader.name=p_path;
                //                    }else{
                //                        deviceDialogLoader.name=name_;
                //                    }
                //                    deviceDialogLoader.stored=stored;
                //                    deviceDialogLoader.type=type_;
                //                    deviceDialogLoader.devices=Devices;
                //                    deviceDialogLoader.active=true;
                //                }else
                if(!stored_ && Devices.length>0){
                    Device= Devices[0]
                    smartConnectLoader.ssid = name_;
                    smartConnectLoader.device = Device;
                    smartConnectLoader.active=true;
                }else if(Devices.length>0){
                    //direct connect
                    Device= Devices[0]
                    backend.connect(p_path,Device);
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
        text: "SHOW ALL"
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
        anchors.right: infoButton.left
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
        visible: false
        text: "VPN"
    }

    CheckBox{
        anchors.right: infoButton.left
        anchors.bottom: parent.bottom
        id: blueshow
        checked: false
        visible: false
        text: "BLUETOOTH"
    }
    Button{
        id: infoButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: FA.icon(FA.fa_info,null);
        font.pixelSize: 18
        background: Rectangle{
            color: "transparent"
        }
        onClicked: {
            if(infoLoader.active === false){
                infoLoader.active = true;
            }else{
                infoLoader.active = false;
            }

        }
    }







}


