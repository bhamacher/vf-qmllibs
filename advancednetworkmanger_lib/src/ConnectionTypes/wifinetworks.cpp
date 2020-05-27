#include "wifinetworks.h"
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WirelessSetting>

WifiNetworks::WifiNetworks()
{
    m_timer = nullptr;
}

WifiNetworks::~WifiNetworks()
{
    if(m_timer != nullptr){
        delete m_timer;
    }
}

bool WifiNetworks::init(ConnectionList  &p_list, DeviceManager &p_devManager)
{
    m_list=&p_list;
    m_devManager=&p_devManager;
    m_type= NetworkManager::Device::Type::Wifi;
    m_setType = NetworkManager::Setting::SettingType::Wireless;

    findStoredConnections();



    for(QString uni : m_devManager->getDevices(m_type)){
        findAvailableConnections(uni);
    }

    connect(m_devManager,&DeviceManager::addDevice,this,&AbstractNetwork::addDevice);
    connect(m_devManager,&DeviceManager::removeDevice,this,&AbstractNetwork::removeDevice);



    for(QString uni : m_devManager->getDevices(NetworkManager::Device::Type::Wifi)){
        DevStruct device;
        device.dev=m_devManager->getDevice(uni).dynamicCast<NetworkManager::WirelessDevice>();
        m_devList[uni]=device;
        device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointAppeared,this,&WifiNetworks::addAccessPoint));
        device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointDisappeared,this,&WifiNetworks::removeAccessPoint));
        findAPs(uni);
    }


    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionAdded, this, &WifiNetworks::addConnection);
    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionRemoved, this, &WifiNetworks::removeConnection);


    connect(NetworkManager::notifier(),&NetworkManager::Notifier::activeConnectionAdded,this,&WifiNetworks::connectionActivated);
    connect(NetworkManager::notifier(),&NetworkManager::Notifier::activeConnectionRemoved,this,&WifiNetworks::connectionDeactivate);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this,&WifiNetworks::updateSignal);
    m_timer->start(500);

    return true;
}




void WifiNetworks::findAPs(QString &p_uni)
{
    NetworkManager::WirelessDevice::Ptr dev;
    NetworkManager::AccessPoint::Ptr ap;

    NetworkManager::AccessPoint::Ptr apInList;
    NetworkManager::Connection::Ptr conInList;
    QString ssid= "";
    QString path = "";
    bool newData=true;

    dev = m_devManager->getDevice(p_uni).dynamicCast<NetworkManager::WirelessDevice>();
    DevStruct device;
    device.dev=dev;
    m_devList[p_uni]=device;
    for(QString path : dev->accessPoints()){
        newData = true;
        ap =  dev->findAccessPoint(path);
        ConStruct conBuf;
        conBuf.metaData.setValue(ConClassType::AP);
        conBuf.con.setValue(ap);
        if(m_conList.contains(path)){
            newData = false;
        }

        for(ConStruct connection: m_conList.values()){
            if(connection.metaData.toInt() == (int)ConClassType::AP){
                if(connection.con.value<NetworkManager::AccessPoint::Ptr>()->ssid()== ap->ssid()){
                    newData=false;
                }
            }
        }


        if(ap->ssid() != ""){
            connectionItem con;
            con.Name=ap->ssid();
            con.Groupe="WIFI";
            con.NmPath=path;
            con.Available=true;
            con.Type=ConType::Wifi;
            con.Connected=false;
            con.Stored = false;
            con.SignalStrength=ap->signalStrength();


            bool found=false;
            for(ConStruct connection: m_conList.values()){
                if(connection.metaData.toInt() != (int)ConClassType::AP){
                    if(connection.con.value<NetworkManager::Connection::Ptr>()->settings()->setting(m_setType).dynamicCast<NetworkManager::WirelessSetting>()->ssid() == ap->ssid()){
                        found=true;
                        connectionItem con2;
                        QString path = connection.con.value<NetworkManager::Connection::Ptr>()->path();
                        con2 = m_list->itemByPath(path);
                        con2.SignalStrength=ap->signalStrength();
                        con2.Available=true;
                        m_list->setItemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path(),con2);
                    }
                }
            }


            if(!found){
                if(newData){
                    m_conList[path]=conBuf;
                    m_list->addItem(con);
                }
                    m_apList[p_uni]=ap->ssid();
            }
        }
    }

}


connectionItem WifiNetworks::CreateConItem(const NetworkManager::Connection::Ptr p_con)
{
    NetworkManager::Setting::Ptr set = p_con->settings()->setting(m_setType);
    connectionItem con;
    con.Name=p_con->name();
    if(set.dynamicCast<NetworkManager::WirelessSetting>()->mode() == NetworkManager::WirelessSetting::NetworkMode::Ap){
        con.Groupe="HOTSPOT";
    }else{
        con.Groupe="WIFI";
    }
    con.NmPath=p_con->path();
    con.Available=false;
    con.Type=ConType::Wifi;
    con.Connected=isConnectionActive(p_con->path());
    con.Stored = true;
    con.SignalStrength=0;
    return con;
}



void WifiNetworks::addAccessPoint(const QString &p_uni)
{
    NetworkManager::AccessPoint::Ptr ap;

    NetworkManager::AccessPoint::Ptr apInList;
    NetworkManager::Connection::Ptr conInList;
    QString ssid = "";
    QString path = "";
    bool newData=true;
    for( DevStruct device : m_devList){
        ap = device.dev.dynamicCast<NetworkManager::WirelessDevice>()->findAccessPoint(p_uni);
        if(ap != nullptr){
            ConStruct conBuf;
            conBuf.metaData.setValue(ConClassType::AP);
            conBuf.con.setValue(ap);
            if(m_conList.contains(p_uni)){
                newData = false;
            }

            for(ConStruct connection: m_conList.values()){
                if(connection.metaData.toInt() == (int)ConClassType::AP){
                    if(connection.con.value<NetworkManager::AccessPoint::Ptr>()->ssid()== ap->ssid()){
                        newData=false;
                    }
                }
            }

            connectionItem con;
            if(ap->ssid() != ""){
                con.Name=ap->ssid();
                con.Groupe="WIFI";
                con.NmPath=p_uni;
                con.Available=true;
                con.Type=ConType::Wifi;
                con.Connected=false;
                con.Stored = false;
                con.SignalStrength=ap->signalStrength();
                bool found=false;
                for(ConStruct connection: m_conList.values()){
                    if(connection.metaData.toInt() != (int)ConClassType::AP){
                        if(connection.con.value<NetworkManager::Connection::Ptr>()->settings()->setting(m_setType).dynamicCast<NetworkManager::WirelessSetting>()->ssid() == ap->ssid()){
                            found=true;
                            connectionItem con2;
                            con2 = m_list->itemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path());
                            con2.SignalStrength=ap->signalStrength();
                            con2.Available=true;
                            m_list->setItemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path(),con2);
                        }
                    }
                }


                if(!found){
                    if(newData){
                        m_conList[p_uni]=conBuf;
                        m_list->addItem(con);
                    }
                }
                    m_apList[p_uni]=ap->ssid();
            }
        }
    }

}

void WifiNetworks::removeAccessPoint(const QString &p_uni)
{
    bool managed=false;
    QString ssid = "";
    if(m_conList.contains(p_uni)){
        for(QMetaObject::Connection qtCon : m_conList[p_uni].qtCons){
            disconnect(qtCon);
        }
        ssid=m_list->itemByPath(p_uni).Name;
        m_conList.remove(p_uni);
        managed=true;
    }else if(m_apList.contains(p_uni)){
        ssid=m_apList[p_uni];
        m_apList.remove(p_uni);
        managed=true;
    }

    if(managed){
    m_list->removeByPath(p_uni);
    for(ConStruct connection: m_conList.values()){
        if(connection.metaData.toInt() != (int)ConClassType::AP){
            if(connection.con.value<NetworkManager::Connection::Ptr>()->settings()->setting(m_setType).dynamicCast<NetworkManager::WirelessSetting>()->ssid() == ssid){
                connectionItem con2;
                con2 = m_list->itemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path());
                if(!con2.Connected){
                    con2.SignalStrength=0;
                    con2.Available=false;
                }
                m_list->setItemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path(),con2);
            }
        }
    }
    }


}

void WifiNetworks::addConnection(const QString &connection)
{
    NetworkManager::Connection::Ptr conPtr = NetworkManager::findConnection(connection);
    QString conSsid="";
    if(connection != nullptr){
        NetworkManager::Setting::Ptr set = conPtr->settings()->setting(m_setType);
        if(set != NULL){
            conSsid=set.staticCast<NetworkManager::WirelessSetting>()->ssid();
            connectionItem conItm= CreateConItem(conPtr);
            for(QString ssid : m_apList){
                if(ssid==conSsid){
                    conItm.Available=true;
                }
            }
            addConnectionToList(conPtr,conItm);
        }
    }


    for(ConStruct conItem :  m_conList){
        if(conItem.metaData.toInt() == (int)ConClassType::AP){
            QString uni=conItem.con.value<NetworkManager::AccessPoint::Ptr>()->uni();
            QString apSsid = conItem.con.value<NetworkManager::AccessPoint::Ptr>()->ssid();
            if(apSsid == conSsid){
                m_conList.remove(uni);
                m_list->removeByPath(uni);
            }

//            removeAccessPoint(uni);
//            addAccessPoint(uni);
            break;
        }
    }
}

void WifiNetworks::removeConnection(const QString &connection)
{
    AbstractNetwork::removeConnection(connection);
    for(QString uni : m_devManager->getDevices(m_type)){
        findAPs(uni);
    }
}


void WifiNetworks::addDevice(NetworkManager::Device::Type p_type, QString p_device){

    if(p_type == m_type){
        NetworkManager::Device::Ptr dev = m_devManager->getDevice(p_device).dynamicCast<NetworkManager::WirelessDevice>();
        DevStruct device;
        device.dev=dev;
        m_devList[p_device]=device;
        device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointAppeared,this,&WifiNetworks::addAccessPoint));
        device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointDisappeared,this,&WifiNetworks::removeAccessPoint));
        findAvailableConnections(p_device);
        findAPs(p_device);
    }
}

void WifiNetworks::updateSignal()
{

    NetworkManager::WirelessDevice::Ptr dev;
    NetworkManager::AccessPoint::Ptr apDev;

    NetworkManager::AccessPoint::Ptr apInList;
    NetworkManager::Connection::Ptr conInList;
    QString ssid;
    QString path;

    for(QString uni : m_devManager->getDevices(NetworkManager::Device::Type::Wifi)){
        dev = m_devManager->getDevice(uni).dynamicCast<NetworkManager::WirelessDevice>();
        QStringList paths = dev->accessPoints();


        for(ConStruct connection: m_conList.values()){
            if(connection.metaData.toInt() == (int)ConClassType::AP ){
                apInList = connection.con.value<NetworkManager::AccessPoint::Ptr>();
                ssid = apInList->ssid();
                path = apInList->uni();

            }else {
                conInList = connection.con.value<NetworkManager::Connection::Ptr>();
                ssid = conInList->settings()->setting(m_setType).dynamicCast<NetworkManager::WirelessSetting>()->ssid();
                path = conInList->path();
            }

            for(QString apPath : paths){
                apDev = dev->findAccessPoint(apPath);
                if(ssid == apDev->ssid()){
                    connectionItem con2;
                    con2 = m_list->itemByPath(path);
                    con2.SignalStrength=apDev->signalStrength();
                    m_list->setItemByPath(path,con2);
                }
            }
        }
    }

}


