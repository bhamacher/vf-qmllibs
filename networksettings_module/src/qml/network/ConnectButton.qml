import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import networksettings 1.0

import "qrc:/src/qml/network"

Pane{
    id: rootItm
    property AbstractNetworkWrapper backend
    property int fontPixelSize
    readonly property real tfratio: 1.9
    Layout.minimumHeight: connect.height
    Layout.minimumWidth: 50
    padding: 0

    Button{
        id: connect
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: fontPixelSize
        text: "APPLY"
        enabled: backend.newSettings
        onClicked: {
            backend.saveCurrentSetting();
        }
    }

}




