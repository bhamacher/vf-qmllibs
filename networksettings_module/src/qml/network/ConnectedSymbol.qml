import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import networksettings 1.0

import "qrc:/src/qml/FontAwesome.js" as FontAwesome


Switch{
    checked: {
        if(backend.conState===AbstractNetworkWrapper.Connected){
            return true;
        }else{
            return false;
        }
    }
    property AbstractNetworkWrapper backend
    property int fontPixelSize
    font.pixelSize: fontPixelSize
    enabled: !backend.newSettings || checked;

    onClicked: {
        if(checked && !backend.newSettings){
            backend.connect = true;
        }else if(checked && backend.newSettings){
            backend.setPopUpMsg("data inconsistent: \nPlease apply your settings before you connect")

        }else{
            backend.connect = false;

        }
    }

    text: {
        if(backend.conState===AbstractNetworkWrapper.Connecting || backend.conState===AbstractNetworkWrapper.Disconnecting){
            animation.running = true;
            return "";
        }else{
            animation.running = false;
            return "";
        }
    }
    SequentialAnimation on text {
        id: animation
        loops: Animation.Infinite
        running: false;
        PropertyAnimation { to: FontAwesome.icon(FontAwesome.fa_refresh,null); duration: 1000}
        PropertyAnimation { to: FontAwesome.icon(FontAwesome.fa_refresh,"blue"); duration: 1000}
    }
}

