import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.3
import networksettings 1.0

import "qrc:/src/qml/network"


Pane{
    id: rootItm
    Layout.minimumHeight: contentHeigh
    Layout.minimumWidth: contentWidth
    width: contentWidth
    height: contentHeight
    property int fontPixelSize



    WiredWrapper{
        id: backend
    }

    GridLayout{
        id: ethTabGrids
        anchors.fill: parent
        anchors.margins: 0
        columnSpacing: 0
        rowSpacing: 0
        columns: 1
        Layout.fillWidth: true



        DeviceList{
            id: devL
            backend: backend
            Layout.fillWidth: true
            fontPixelSize: rootItm.fontPixelSize
        }

        Ipv4Input{
            id: ipv4
            Layout.fillWidth: true
            backend: backend
            Layout.margins: 0
            fontPixelSize: rootItm.fontPixelSize
        }

        Ipv6Input{
            id: ipv42
            Layout.fillWidth: true
            backend: backend
            Layout.margins: 0
            fontPixelSize: rootItm.fontPixelSize
        }
        ConnectButton{
            id: connectioncontrols
            Layout.fillWidth: true
            backend: backend
            fontPixelSize: rootItm.fontPixelSize

            ConnectedSymbol{
                id: connected
                backend: backend
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                fontPixelSize: parent.fontPixelSize
            }

        }

    }



}
