import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {
    id: rootItm
    title: "Wifi Password"
    RowLayout{
        width: parent.width
        Label{
            id: pwLabel
            text: "NETWORKS: "
        }

        ComboBox{
            id: pwInput
            Layout.fillWidth: true
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

}


