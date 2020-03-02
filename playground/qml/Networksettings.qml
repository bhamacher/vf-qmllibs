import QtQuick 2.0

import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

// Load our plugin from filesystem
import networksettings 1.0



ApplicationWindow {
    id: mainWindow
    visible: true
    title: "networkmanager"


    // resolution the gui was designed with
    readonly property int defWidth : 800
    readonly property int defHeight : 400

    width: 800
    height: 480

    //    width: 1024
    //    height: 600

    //the actual dispaly size compared to the development screen
    readonly property real ratioWidth : 1
    readonly property real ratioHeight : 1

    // the determined pixelsize. The defualt size is 16. This will be scaled by the smallest value but not smaller than 0.7.
    readonly property int globPixelSize : 16*Math.max(0.7,Math.min((width/defWidth*ratioWidth),(height/defHeight*ratioWidth)))


    //Material.accent: Material.Green


    Flickable{
        anchors.fill: parent
        anchors.margins: 0
        //contentHeight: mainFrame.contentHeight
        ScrollBar.vertical: ScrollBar {
            active:{if((mainFrame.contentHeight)>height){
                    return true;
                }else{
                    return false;
                }
            }
        }

        Frame{
            id: mainFrame
            anchors.fill: parent
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 0
            Layout.minimumHeight: contentHeight
            Layout.minimumWidth: contentWidth


            TabBar {
                id: bar
                width: parent.width
                anchors.top: parent.top
                TabButton {
                    text: qsTr("ETHERNET")
                }
                TabButton {
                    text: qsTr("WIFI")
                }
            }




            StackLayout
            //SwipeView
            {
                id: stack
                Layout.minimumWidth: contentWidth
                //width: parent.width
                //height: parent.height - bar.height
                currentIndex: bar.currentIndex
                anchors.top: bar.bottom
                //anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 0


                //                Flickable{
                //                    id: flick1
                //                    anchors.fill: parent
                //                    anchors.margins: 0
                //                    contentHeight: ethTab.contentHeight
                //                    z:-1

                EthernetTab{
                    id: ethTab
                    width: parent.width
                    Layout.fillHeight: true
                    height: flick1.height
                    fontPixelSize: mainWindow.globPixelSize
                }

                //                    ScrollBar.vertical: ScrollBar {
                //                        active:{if((ethTab.contentHeight-43)>flick1.height){
                //                                return true;
                //                            }else{
                //                                return false;
                //                            }
                //                        }
                //                    }
                //                }

                //                Flickable{
                //                    id: flick2
                //                    anchors.fill: parent
                //                    anchors.margins: 0
                //                    contentHeight: wifiTab.contentHeight
                //                    z:-1

                                    WifiTab{
                                        id: wifiTab
                                        width: parent.width
                                        height: flick2.height
                                        fontPixelSize: mainWindow.globPixelSize
                                    }
                //                    ScrollBar.vertical: ScrollBar {
                //                        active:{if((ethTab.contentHeight-43)>flick1.height){
                //                                return true;
                //                            }else{
                //                                return false;
                //                            }
                //                        }
                //                    }
                //                }


            }


        }
    }

}


