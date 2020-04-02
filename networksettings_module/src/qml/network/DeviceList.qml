import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import networksettings 1.0



ComboBox {
    id: box
    property AbstractNetworkWrapper backend
    property int fontPixelSize
    font.pixelSize: fontPixelSize
    model: backend.deviceList
    visible : {
        if(count >= 0){
            return true;
        }else{
            return false;
        }
    }
    onCurrentIndexChanged: {
        if(currentIndex!=-1){
            backend.setCurrentDevice(model[currentIndex])
        }
    }
    Component.onCompleted: {
        backend.setCurrentDevice(currentText)
    }

}
