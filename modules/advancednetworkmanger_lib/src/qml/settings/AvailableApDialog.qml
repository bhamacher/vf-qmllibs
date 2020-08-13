import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import anmsettings 1.0

Dialog {
    id: rootItm
    title: "Wifi Password"

    signal okPressed(string retSsid)

    NetworkmanagerAbstraction{
        id: backend
    }
    RowLayout{
        width: parent.width
        Label{
            id: pwLabel
            text: "NETWORKS: "
        }
        ComboBox{
            id: ssidList
            Layout.fillWidth: true
            model: backend.aps
        }
    }
    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
        okPressed(ssidList.currentText)
    }

}


