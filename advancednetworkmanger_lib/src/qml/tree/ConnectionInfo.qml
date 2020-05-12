import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import anmsettings 1.0


Pane {
//    InfoInterface{
//        id: backend
//    }

    ListView{
        id: list
        anchors.top: parent.top
        anchors.bottom: showall.top
        anchors.left: parent.left
        anchors.right: parent.right
        clip: true
        model: backend.activeConnections
    }






}
