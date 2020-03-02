import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import networksettings 1.0

Popup {
    id: popup
    property AbstractNetworkWrapper backend
    readonly property real tfratio: 1.9
    property int fontPixelSize
    anchors.centerIn: parent
    Layout.fillWidth:true
    height: 200
    modal: true
    focus: true
    property string msg: backend.popupMsg
    closePolicy: popup.CloseOnPressOutside
    contentItem: Label {
        text: popup.msg
        font.pixelSize: rootItm.fontPixelSize
    }
    onMsgChanged: {
        open();
    }
}

