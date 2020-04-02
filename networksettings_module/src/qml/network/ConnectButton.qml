import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import networksettings 1.0

import "qrc:/src/qml/network"
Pane{
    id: root
    property AbstractNetworkWrapper backend
    property int fontPixelSize
    readonly property real tfratio: 1.9

Button{
    id: connect
    property int fontPixelSize
    readonly property real tfratio: 1.9
    width: parent.width
    anchors.verticalCenter: parent.verticalCenter
    font.pixelSize: fontPixelSize
    text: "APPLY"
    enabled: backend.newSettings;
    onClicked: {
        backend.saveCurrentSetting();
    }
}
}






