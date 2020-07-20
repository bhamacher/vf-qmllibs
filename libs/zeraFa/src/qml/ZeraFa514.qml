pragma Singleton

import QtQuick 2.0
import QtQuick.Controls 2.0
import "qrc:/src/qml/"

Item {
    id: zerafa

    property alias icons: variables

    readonly property FontLoader fontAwesomeRegular: FontLoader {
        source: "qrc:/3rdParty/Font-Awesome/webfonts/fa-regular-400.ttf"
    }
    readonly property FontLoader fontAwesomeSolid: FontLoader {
        source: "qrc:/3rdParty/Font-Awesome/webfonts/fa-solid-900.ttf"
    }
    readonly property FontLoader fontAwesomeBrands: FontLoader {
        source: "qrc:/3rdParty/Font-Awesome/webfonts/fa-brands-400.ttf"
    }

    readonly property string regular: fontAwesomeRegular.name
    readonly property string solid: fontAwesomeSolid.name
    readonly property string brands: fontAwesomeBrands.name


    FaIcons514 {
        id: variables
    }

    function icon(symbols, color) {
        var colorStart="";
        var colorEnd="";
        if(color!=null) //implicit conversion is intentional
        {
            colorStart="<font color='"+color+"'>";
            colorEnd="</font>";
        }
        return colorStart+symbols+" "+colorEnd;
    }
}
