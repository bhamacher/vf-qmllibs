import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import anmsettings 1.0
import "qrc:/src/qml/FontAwesome.js" as FA

Dialog {
    id: rootItm
    title: "WIFI PASSWORD"
    property string device: ""

    function init(ssid,device){
        backend.create();
        rootItm.device=device;
        backend.conName = ssid;
        backend.ssid = ssid;
        backend.mode = "CLIENT";
       // backend.device = device;
    }


    WirelessConnectionSettingsInterface{
        id: backend
    }



        RowLayout{
            id: pwInput
            width: parent.width
            Label{
                id: pwLabel
                text: "Password: "
            }

            TextField{
                id: pw
                echoMode: TextInput.Password
                Layout.fillWidth: true
                onEditingFinished: {
                    backend.password=text;
                }
            }

            Button{
                id: pwvisible
                text: FA.icon(FA.fa_eye_slash,null)
                font.pixelSize: rootItm.fontPixelSize
                background: Rectangle{
                    color: "transparent"
                }
                onPressed: {
                    pw.echoMode = TextInput.Normal
                    pwvisible.text= FA.icon(FA.fa_eye,null)
                }
                onReleased: {
                    pw.echoMode = TextInput.Password
                    pwvisible.text= FA.icon(FA.fa_eye_slash,null)
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
