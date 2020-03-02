import QtQuick 2.8
import QtQuick.Controls 2.0






Button {
    text: "Click me!"
    onClicked: {
        console.debug("This is in the qml lib");
        pluginItem.color = pluginItem.color+20;
        //wifiInterface.networkName=
    }
}
