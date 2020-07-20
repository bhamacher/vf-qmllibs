import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import networksettings 1.0

import "qrc:/src/qml/network"


Pane{
    id: rootItm
    property int fontPixelSize



    WifiWrapper{
        id: backend

    }




    GridLayout{
        id: wifiTabGrid
        anchors.fill: parent
        anchors.margins: 0
        columns: 1

        DeviceList{
            id: devL
            Layout.fillWidth: true
            backend: backend
            fontPixelSize: rootItm.fontPixelSize
        }

        AvailableNetworks {
            id: avnw
            Layout.fillWidth: true
            Layout.fillHeight: true
            backend: backend
            fontPixelSize: rootItm.fontPixelSize
        }

        Networkname {
            id: test3
            Layout.fillHeight: true
            Layout.fillWidth: true
            backend: backend
            fontPixelSize: rootItm.fontPixelSize
        }

        ConnectButton{
            id: connectioncontrols
            Layout.fillWidth: true
            Layout.fillHeight:  true
            backend: backend
            fontPixelSize: rootItm.fontPixelSize


            ConnectedSymbol{
                id: connect
                backend: backend
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillHeight: true
                fontPixelSize: rootItm.fontPixelSize
            }


            Label {
                id: connetworkname
                text: backend.conNetwork
                anchors.verticalCenter: connect.verticalCenter
                anchors.right: connect.left
                font.pixelSize: rootItm.fontPixelSize
            }
        }

        UniversalMsgPopup{
            id: popUp
            fontPixelSize: rootItm.fontPixelSize
            backend: backend
        }


}
}



