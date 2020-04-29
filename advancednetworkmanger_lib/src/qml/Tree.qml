import QtQuick 2.0

Row {
    property int level
    anchors.left: parent.left
    anchors.right: parent. right
    anchors.leftMargin: level*20
}
