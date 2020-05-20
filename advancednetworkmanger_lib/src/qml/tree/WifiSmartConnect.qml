import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import anmsettings 1.0
import "qrc:/src/qml/FontAwesome.js" as FA

Dialog {
    id: rootItm
    title: "Wifi Password"
    property string device: ""

    function init(ssid,device){
        backend.create();
        rootItm.device=device;
        backend.conName = ssid + " stored";
        backend.ssid = ssid;
        backend.mode = "CLIENT";
    }


    WirelessConnectionSettingsInterface{
        id: backend
    }


    RowLayout{
        width: parent.width
        Label{
            id: pwLabel
            text: "Password: "
        }

        TextField{
            id: pwInput
            Layout.fillWidth: true
            onEditingFinished: {
                backend.password=text;
            }
        }
    }

    onAccepted: {
        backend.saveAndActivate(rootItm.device);
    }

    onDiscarded: {
        backend.discard();
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

}
