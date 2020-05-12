#include "advancednetworkmanager.h"




#include <NetworkManagerQt/Device>

#include <QCoreApplication>
#include <QtQml/QtQml>

void AdvancedNetworkmanager::init()
{
    AdvancedNetworkmanager* obj= new AdvancedNetworkmanager();
    obj->registerTypes("anmsettings");
}



void AdvancedNetworkmanager::registerTypes(const char* uri) {
    qmlRegisterType<AbstractConnectionSettingsInterface>(uri, 1, 0, "AbstractConnectionSettingsInterface");
    qmlRegisterType<ConnectionModel>(uri, 1, 0, "ConnectionModel");
   /* qmlRegisterType<ConModelItem>(uri, 1, 0, "ConModelItem")*/;
    qmlRegisterType<ConnectionTreeInterface>(uri, 1, 0, "ConnectionTreeInterface");
    qmlRegisterType<WiredConnectionSettingsInterface>(uri, 1, 0, "WiredConnectionSettingsInterface");
    qmlRegisterType<WirelessConnectionSettingsInterface>(uri, 1, 0, "WirelessConnectionSettingsInterface");
//    qmlRegisterUncreatableType<NetworkManager::Device>(uri, 1, 0, "NmQtDevice","");
}


static void init(){
    AdvancedNetworkmanager::init();
}
Q_COREAPP_STARTUP_FUNCTION(init)
