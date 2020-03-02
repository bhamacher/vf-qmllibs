#include "vfnetworkmanager.h"
#include <iostream>
#include <QtQml/QtQml>


#include "abstractnetworkwrapper.h"
#include "wifiwrapper.h"
#include "wiredwrapper.h"

void VfNetworkManager::registerTypes(const char* uri) {
           qDebug() << "networksettings loaded";
        // Register types for use in qml
        qmlRegisterType<AbstractNetworkWrapper>(uri, 1, 0, "AbstractNetworkWrapper");
        qmlRegisterType<WiredWrapper>(uri, 1, 0, "WiredWrapper");
        qmlRegisterType<WifiWrapper>(uri, 1, 0, "WifiWrapper");
}
