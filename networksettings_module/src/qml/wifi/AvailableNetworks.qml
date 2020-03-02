import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import networksettings 1.0

import "qrc:/src/qml/network"

import "qrc:/src/qml/FontAwesome.js" as FontAwesome

Pane{
    id: rootItm
    property WifiWrapper backend
    readonly property real tfratio: 1.9
    property int fontPixelSize
    Layout.minimumHeight: contentHeight
    Layout.minimumWidth: contentWidth
    padding: 0

    FontLoader {
        source: "qrc:../3rdparty/font-awesome-4.6.1/fonts/fontawesome-webfont.ttf"
    }

    GridLayout{
        anchors.fill: parent
        columns: 1

        RadioButton {
            id: ncrb
            text: qsTr("NETWORKCLIENT")
            Layout.fillWidth: true
            font.pixelSize: rootItm.fontPixelSize
            checked: {
                if(backend.operationMode==WifiWrapper.Client){
                    return true;
                }else{
                    return false;
                }
            }
            onClicked: {
                backend.operationMode=WifiWrapper.Client;
            }
        }

        GridLayout{
            Layout.leftMargin: parent.width/8
            columnSpacing :2
            rowSpacing :2
            Layout.margins: 0
            columns:3

            Label {
                id: networkLabel
                text: qsTr("NETWORKS")+":"
                font.pixelSize: rootItm.fontPixelSize
                verticalAlignment: Text.AlignVCenter
                //Layout.fillHeight: true
//                Layout.fillWidth: true

            }

            ComboBox {
                id: box
                Layout.fillWidth: true
                model: backend.availableNetworks
                currentIndex: indexOfValue(backend.currentNetwork)
                font.pixelSize: rootItm.fontPixelSize
                Layout.preferredHeight: refreshButton.height
                onCurrentIndexChanged: {
                    backend.currentNetwork=model[currentIndex];
                    if(model[currentIndex]===backend.currentNetwork){

                    }
                }
                onCountChanged: {
                    backend.currentNetwork=model[currentIndex];
                }
            }


            Button{
                id: refreshButton
                text: FontAwesome.icon(FontAwesome.fa_refresh,null)
//                anchors.left: box.right
//                anchors.verticalCenter: box.verticalCenter
                font.pixelSize: rootItm.fontPixelSize
                background: Rectangle{
                    color: "transparent"
                }

                onClicked: {
                    backend.refreshNetworks();
                    animation.running=true;
                }
                SequentialAnimation on text {
                        id: animation
                        loops: 5
                        running: false;
                        PropertyAnimation { to: FontAwesome.icon(FontAwesome.fa_refresh,"blue"); duration: 1000}
                        PropertyAnimation { to: FontAwesome.icon(FontAwesome.fa_refresh,null); duration: 1000}

                    }
            }

            Label {
                id: pwLabel
                text: qsTr("PASSWORD")+":"
                font.pixelSize: rootItm.fontPixelSize
                verticalAlignment: Text.AlignVCenter
                //Layout.fillHeight: true
//                Layout.fillWidth: true
            }

            TextField {
                id: pw
                placeholderText: qsTr("PASSWORD")
                //validator: RegExpValidator { regExp: /^[a-zA-Z1-9]{8,63}$/}
                Layout.fillWidth: true
                //Layout.fillHeight: true
                font.pixelSize: rootItm.fontPixelSize
                text: {
                      if(!focus){
                        backend.apLoginPassword;
                      }
                }
                Layout.minimumHeight: rootItm.tfratio*contentHeight
                echoMode: TextInput.Password
                onEditingFinished: {
                    backend.apLoginPassword = text;
                }

            }

            Button{
                id: pwvisible
                text: FontAwesome.icon(FontAwesome.fa_eye_slash,null)
                font.pixelSize: rootItm.fontPixelSize
                background: Rectangle{
                    color: "transparent"
                }

                onPressed: {
                    pw.echoMode = TextInput.Normal
                    pwvisible.text= FontAwesome.icon(FontAwesome.fa_eye,null)
                }
                onReleased: {
                    pw.echoMode = TextInput.Password
                    pwvisible.text= FontAwesome.icon(FontAwesome.fa_eye_slash,null)
                }
            }


        }



    }

}
