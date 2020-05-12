#include "wifinetworks.h"
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WirelessSetting>

WifiNetworks::WifiNetworks()
{

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
        findAPs(uni);
    }

    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionAdded, this, &WifiNetworks::addConnection);
    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionRemoved, this, &WifiNetworks::removeConnection);


    connect(NetworkManager::notifier(),&NetworkManager::Notifier::activeConnectionAdded,this,&WifiNetworks::connectionActivated);
    connect(NetworkManager::notifier(),&NetworkManager::Notifier::activeConnectionRemoved,this,&WifiNetworks::connectionDeactivate);

    return true;
}




void WifiNetworks::findAPs(QString &p_uni)
{
    NetworkManager::WirelessDevice::Ptr dev;
    NetworkManager::AccessPoint::Ptr ap;
    bool newData=true;

    dev = m_devManager->getDevice(p_uni).dynamicCast<NetworkManager::WirelessDevice>();
    DevStruct device;
    device.dev=dev;
    device.qtCons.append(connect(device.dev.data(),&NetworkManager::Device::availableConnectionAppeared,this,&WifiNetworks::addConnection));
    device.qtCons.append(connect(device.dev.data(),&NetworkManager::Device::availableConnectionDisappeared,this,&WifiNetworks::removeConnection));
    device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointAppeared,this,&WifiNetworks::addAccessPoint));
    device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointDisappeared,this,&WifiNetworks::removeAccessPoint));
    m_devList[p_uni]=device;
    for(QString path : dev->accessPoints()){
        ap =  dev->findAccessPoint(path);
        ConStruct conBuf;
        conBuf.metaData.setValue(ConClassType::AP);
        conBuf.con.setValue(ap);
        if(m_conList.contains(path)){
            newData = false;
        }
        //m_conList[path]=conBuf;
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
                    con = m_list->itemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path());
                    con.SignalStrength=ap->signalStrength();
                    m_list->setItemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path(),con);
                }
            }
        }


        if(!found){
            m_conList[path]=conBuf;
            if(newData){
                m_list->addItem(con);
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

            connectionItem con;
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
                        con = m_list->itemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path());
                        con.SignalStrength=ap->signalStrength();
                        m_list->setItemByPath(connection.con.value<NetworkManager::Connection::Ptr>()->path(),con);
                    }
                }
            }

            if(!found){
                m_conList[p_uni]=conBuf;
                if(newData){
                    m_list->addItem(con);
                }
            }

        }
    }

}

void WifiNetworks::removeAccessPoint(const QString &p_uni)
{
    if(m_conList.contains(p_uni)){
        for(QMetaObject::Connection qtCon : m_conList[p_uni].qtCons){
            disconnect(qtCon);
        }
        m_conList.remove(p_uni);
        m_list->removeByPath(p_uni);
    }
}

void WifiNetworks::addConnection(const QString &connection)
{
    AbstractNetwork::addConnection(connection);
    for(ConStruct conItem :  m_conList){
        if(conItem.metaData.toInt() == (int)ConClassType::AP){
            removeAccessPoint(conItem.con.value<NetworkManager::AccessPoint::Ptr>()->uni());
            addAccessPoint(conItem.con.value<NetworkManager::AccessPoint::Ptr>()->uni());
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


void WifiNetworks::addDevice(NetworkManager::Device::Type p_type, QString p_device)
{

    if(p_type == m_type){
        NetworkManager::Device::Ptr dev = m_devManager->getDevice(p_device).dynamicCast<NetworkManager::WirelessDevice>();

        DevStruct device;
        device.dev=dev;
        m_devList[p_device]=device;
        device.qtCons.append(connect(device.dev.data(),&NetworkManager::Device::availableConnectionAppeared,this,&WifiNetworks::addConnection));
        device.qtCons.append(connect(device.dev.data(),&NetworkManager::Device::availableConnectionDisappeared,this,&WifiNetworks::removeConnection));
        device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointAppeared,this,&WifiNetworks::addAccessPoint));
        device.qtCons.append(connect(device.dev.dynamicCast<NetworkManager::WirelessDevice>().data(),&NetworkManager::WirelessDevice::accessPointDisappeared,this,&WifiNetworks::removeAccessPoint));

        findAvailableConnections(p_device);
        findAPs(p_device);
    }
}


