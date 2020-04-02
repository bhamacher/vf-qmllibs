import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import networksettings 1.0

//Do not add minimum height
ComboBox {
    id: root
    property WiredWrapper backend
    property int fontPixelSize
    property string ipType //: "ipv4"
    font.pixelSize: fontPixelSize
//    Layout.minimumWidth: model[currentIndex].length*fontPixelSize
//    Layout.minimumHeight: model[currentIndex].height*fontPixelSize
    model: ["AUTOMATIC", "MANUAL"]
//    currentIndex:
     Component.onCompleted: {
        var ret=0;
        if(ipType==="ipv4"){

            if(WiredWrapper.Automatic===backend.currentIpv4ConType){
                ret= 0;
            }else if(WiredWrapper.Manual===backend.currentIpv4ConType){
                 ret= 1;
            }

        }else if(ipType==="ipv6"){
            if(WiredWrapper.Automatic===backend.currentIpv6ConType){
                 ret= 0;
            }else if(WiredWrapper.Manual===backend.currentIpv6ConType){
                 ret= 1;
            }

        }
        currentIndex =  ret;
    }


    onCurrentIndexChanged: {
        if(ipType==="ipv4"){

            if('MANUAL'===model[currentIndex]){
//                backend.setCurrentIpv4ConType(WiredWrapper.Manual)
                backend.currentIpv4ConType = WiredWrapper.Manual
            }else{
//                backend.setCurrentIpv4ConType(WiredWrapper.Automatic)
                backend.currentIpv4ConType = WiredWrapper.Automatic
            }
        }else if(ipType==="ipv6"){
            if('MANUAL'===model[currentIndex]){
//                backend.setCurrentIpv6ConType(WiredWrapper.Manual)
                backend.currentIpv6ConType = WiredWrapper.Manual
            }else{
//                backend.setCurrentIpv6ConType(WiredWrapper.Automatic)
                backend.currentIpv6ConType = WiredWrapper.Automatic
            }
        }


    }
}


