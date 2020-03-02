import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import networksettings 1.0

//Do not add minimum height
ComboBox {
    id: box
    property WiredWrapper backend
    property int fontPixelSize
    property string ipType //: "ipv4"
    font.pixelSize: fontPixelSize
    Layout.minimumWidth: model[currentIndex].length*fontPixelSize
    Layout.minimumHeight: model[currentIndex].height*fontPixelSize
    model: ["AUTOMATIC", "MANUAL"]
    currentIndex: {
        if(ipType==="ipv4"){

            if(WiredWrapper.Automatic===backend.currentIpv4ConType){
                return 0;
            }else if(WiredWrapper.Manual===backend.currentIpv4ConType){
                return 1;
            }

        }else if(ipType==="ipv6"){
            if(WiredWrapper.Automatic===backend.currentIpv6ConType){
                return 0;
            }else if(WiredWrapper.Manual===backend.currentIpv6ConType){
                return 1;
            }

        }
    }
    onCurrentIndexChanged: {
        if(ipType==="ipv4"){

            if('MANUAL'===model[currentIndex]){
                box.backend.currentIpv4ConType = WiredWrapper.Manual
            }else{
                box.backend.currentIpv4ConType = WiredWrapper.Automatic
            }
        }else if(ipType==="ipv6"){
            if('MANUAL'===model[currentIndex]){
                box.backend.currentIpv6ConType = WiredWrapper.Manual
            }else{
                box.backend.currentIpv6ConType = WiredWrapper.Automatic
            }
        }


    }
}


