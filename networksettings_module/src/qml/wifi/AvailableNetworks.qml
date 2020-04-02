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

        Grid{
            id: indentation
            columns: 2
            Layout.fillWidth: true

            Item{
                id: spacer
                width: parent.width/8
                height:1
            }

            GridLayout{
                width: parent.width-spacer.width-10
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
                    font.pixelSize: rootItm.fontPixelSize
                    Layout.preferredHeight: refreshButton.height
                    onCurrentIndexChanged: {
                        if(currentIndex<count && currentIndex>=0){
                            backend.currentNetwork=model[currentIndex];
                        }
                    }
                    onCountChanged: {
                        if(currentIndex<count && currentIndex>=0){
                            backend.currentNetwork=model[currentIndex];
                        }
                    }
                }


                Button{
                    id: refreshButton
                    text: FontAwesome.icon(FontAwesome.fa_refresh,null)
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
                }

                TextField {
                    id: pw
                    placeholderText: qsTr("PASSWORD")
                    Layout.fillWidth: true
                    font.pixelSize: rootItm.fontPixelSize
                    text: {
                        backend.apLoginPassword;
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

}
