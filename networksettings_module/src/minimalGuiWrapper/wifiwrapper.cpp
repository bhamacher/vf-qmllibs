#include "wifiwrapper.h"

WifiWrapper::WifiWrapper(QObject *parent): AbstractNetworkWrapper(parent)
{
    connect(this, &AbstractNetworkWrapper::initalized,this, &WifiWrapper::init);
}

WifiWrapper::~WifiWrapper()
{

}



void WifiWrapper::init()
{

    m_techList.clear();

    setAppState(AppletStatus::Initalize);

    // Get wifi devices from device list
    Q_FOREACH (NetworkManager::Device::Ptr dev, m_list) {
        if(NM_DEVICE_TYPE_WIFI == dev->type()){
            m_techList.append(dev);
        }
    }

    //set current device
    if(m_techList.size()>0){
        m_currentDevice=m_techList[0];

    }

    //Do rest of init
    reinit();


    //set state to Input
    setAppState(AppletStatus::Input);
}

void WifiWrapper::reinit()
{

    if(m_currentDevice!=nullptr){


        NetworkManager::ActiveConnection::Ptr activeCon;
        activeCon=m_currentDevice->activeConnection();
        QString clientPath="";

        //search for wireless client connection Template in NM
        Q_FOREACH(NetworkManager::Connection::Ptr connection, NetworkManager::listConnections()){
            if(connection->name() == getExpectedClientName()){
                clientPath=connection->path();
            }
        }

        //if there is no connection add it
        if(""==clientPath){
            clientPath  = addConnection(getDefaultClientConnection()).value().path();
        }


        //get the new connection from NM
        m_clientCon=NetworkManager::findConnection(clientPath);
        QDBusPendingReply<NMVariantMapMap> map=m_clientCon->secrets("802-11-wireless-security");
        m_password=map.value()["802-11-wireless-security"]["psk"].toString();

        //search for access point connection template in NM
        QString accessPointPath="";
        Q_FOREACH(NetworkManager::Connection::Ptr connection, NetworkManager::listConnections()){
            if(connection->name() == getExpectedAccessPointName()){
                accessPointPath=connection->path();
            }
        }

        // If there is no connetion add it
        if(""==accessPointPath){
            accessPointPath  = addConnection(getDefaultAccessPointConnection()).value().path();
        }


        // get the new connection from NM
        m_accessPointCon=NetworkManager::findConnection(accessPointPath);
        map=m_accessPointCon->secrets("802-11-wireless-security");
        m_apPassword=map.value()["802-11-wireless-security"]["psk"].toString();

        // set current connection to active connection.
        // if there is no active connection set it to client connection
        if(nullptr != activeCon){
            if(activeCon->uuid()==m_accessPointCon->uuid() && (getWifiDevice()->wirelessCapabilities() & NetworkManager::WirelessDevice::ApCap)){
                m_currentConnection=m_accessPointCon;

            }else{
                m_currentConnection=m_clientCon;
            }
        }else{
            m_currentConnection=m_clientCon;
        }

        //connect signals to current device and connection
        reconnectSignals();

        //get connection status
        assumeConState();
    }

    //refresh gui
    refresh();
    emit AvailableNetworksChanged();
}



void WifiWrapper::reconnectSignals()
{
    // check if there is a device and connection to connect to
    if(m_currentDevice!=nullptr && m_currentConnection!=nullptr){
        // remove all signal/slot connection froom the old devcice
        Q_FOREACH(QMetaObject::Connection qtcon, qtConnections){
            disconnect(qtcon);
        }
        //clear signal/slot connection list
        qtConnections.clear();
        //etablish new onnection sand store them in list.
        qtConnections.push_back(connect(m_currentConnection.data(),&NetworkManager::Connection::updated,this,&AbstractNetworkWrapper::updated));
        qtConnections.push_back(connect(m_currentDevice.data(),&NetworkManager::Device::stateChanged,this,&AbstractNetworkWrapper::DeviceConStateChanged));
        qtConnections.push_back(connect(getWifiDevice().data(),&NetworkManager::WirelessDevice::accessPointAppeared,this,&WifiWrapper::AccessPointAppeared));
        qtConnections.push_back(connect(getWifiDevice().data(),&NetworkManager::WirelessDevice::accessPointDisappeared,this,&WifiWrapper::AccessPointAppeared));
        qtConnections.push_back(connect(m_currentDevice->activeConnection().data(),&NetworkManager::ActiveConnection::stateChanged,this,&AbstractNetworkWrapper::ActiveConStateChanged));
    }
}

void WifiWrapper::refresh()
{
    emit CurrentNetworkChanged();
    emit ApNameChanged();
    emit ApPasswordChanged();
    emit OperationModeChanged();
    emit ApLoginPasswordChanged();
    emit IsConnectedChanged();
    emit DeviceListChanged();
    emit ConnectedNetworkChanged();
}

NMVariantMapMap WifiWrapper::getDefaultClientConnection()
{
    /* These are the defualt properties for an client connection */
    NMVariantMapMap connection;
    connection["connection"]["uuid"] = QUuid::createUuid().toString().remove('{').remove('}');
    connection["connection"]["id"] = getExpectedClientName();
    connection["connection"]["type"] = "802-11-wireless";
    connection["ipv4"]["method"] = "auto";
    connection["ipv6"]["method"] = "auto";
    connection["802-11-wireless"]["ssid"] = QByteArray(QString("dummy").toUtf8());
    connection["802-11-wireless"]["mode"] = "infrastructure";
    connection["802-11-wireless"]["security"]="802-11-wireless-security";
    connection["802-11-wireless-security"]["key-mgmt"]= "wpa-psk";
    connection["802-11-wireless-security"]["psk"]= "abcd1234";
    return connection;
}

NMVariantMapMap WifiWrapper::getDefaultAccessPointConnection()
{
    /* These are the default properties for an access point */
    NMVariantMapMap connection;
    connection["connection"]["uuid"] = QUuid::createUuid().toString().remove('{').remove('}');
    connection["connection"]["id"] = getExpectedAccessPointName();
    connection["connection"]["type"] = "802-11-wireless";
    connection["ipv4"]["method"] = "auto";
    connection["ipv6"]["method"] = "auto";
    connection["802-11-wireless"]["ssid"] = QByteArray(QString("ZeraDevice").toUtf8());
    connection["802-11-wireless"]["mode"] = "ap";
    connection["802-11-wireless"]["security"]="802-11-wireless-security";
    connection["802-11-wireless-security"]["key-mgmt"]= "wpa-psk";
    connection["802-11-wireless-security"]["psk"]= "abcd1234";
    return connection;
}

QString WifiWrapper::getExpectedClientName()
{
    // name of the client connection in NM
    return "WifiConnectionClient";
}

QString WifiWrapper::getExpectedAccessPointName()
{
    // name of the access point connection in NM
    return "WifiConnectionAccessPoint";
}


QStringList WifiWrapper::getAvailableNetworks()
{
    QStringList ssidList;
    if(nullptr==m_currentDevice)return QStringList("empty");
    Q_FOREACH(QString ap, getWifiDevice()->accessPoints()){
        NetworkManager::AccessPoint accessPoint(ap);
        ssidList.append(accessPoint.ssid());
    }
    return ssidList;
}



QString WifiWrapper::getCurrentNetwork()
{
    if(nullptr==m_currentDevice)return noAccess;
    if(nullptr==getCurrentWirelessClientSettings())return noAccess;
    return getCurrentWirelessClientSettings()->ssid();
}

void WifiWrapper::setCurrentNetwork(QString p_currentNetwork)
{
    if(itemsAccessable()){
        if(getOperationMode()==WifiMode::Client){
            getCurrentWirelessSettings()->setSsid(QByteArray(p_currentNetwork.toUtf8()));
            setNewSettings(true);
        }
    }
    emit CurrentNetworkChanged();

}

QString WifiWrapper::getConnectedNetwork()
{
    QString ret="";
    if(m_currentDevice!=nullptr){
        if(m_currentDevice->activeConnection()!=nullptr){
            auto con=m_currentDevice->activeConnection()->connection();
            auto set=con->settings()->setting(NetworkManager::Setting::Wireless).staticCast<NetworkManager::WirelessSetting>();
            ret= set->ssid();
        }
    }
    return ret;
}

QString WifiWrapper::getApName()
{
    QString ret=noAccess;

    if(nullptr!=m_currentDevice){
        if(nullptr!=getCurrentWirelessApSettings()){
            ret=getCurrentWirelessApSettings()->ssid();
        }else{
            ret=getAvailableNetworks()[0];
        }
    }
    return ret;
}

void WifiWrapper::setApName(QString ApName)
{
    if(getOperationMode()!=WifiMode::Hotspot)return;
    if(nullptr==m_currentDevice)return;
    if(m_currentConnection!=nullptr){
        getCurrentWirelessApSettings()->setSsid(QByteArray(ApName.toUtf8()));
    }
    setNewSettings(true);
    emit ApNameChanged();

}

QString WifiWrapper::getApLoginPassword()
{
    //    return m_password;
    if(m_clientCon==nullptr)return "";
    return m_password;
}

void WifiWrapper::setApLoginPassword(QString ApPassword)
{
    if(nullptr!=m_currentDevice && m_currentConnection!=nullptr){
        if(getOperationMode()==WifiMode::Client){
            // write new password to NM and to locl memeory, b
            // NM will not give us the password back. Do not know why.
            getCurrentWirelessSecurityClientSettings()->setPsk(ApPassword);
            m_password=ApPassword;
            setNewSettings(true);
        }
    }
    emit ApLoginPasswordChanged();
}

QString WifiWrapper::getApPassword()
{
    // return password from local memory.
    // NM will not give it to us
    return m_apPassword;
}

void WifiWrapper::setApPassword(QString ApPassword)
{
    if(nullptr!=m_currentDevice && m_currentConnection!=nullptr){
        if(getOperationMode()==WifiMode::Hotspot){
            getCurrentWirelessSecuritySettings()->setPsk(ApPassword);
            m_apPassword=ApPassword;
            setNewSettings(true);
        }
    }
    emit ApPasswordChanged();

}

WifiWrapper::WifiMode WifiWrapper::getOperationMode()
{
    WifiMode ret = WifiMode::Client;
    if(itemsAccessable()){

        if(NetworkManager::WirelessSetting::Ap==getCurrentWirelessSettings()->mode()){
            ret=WifiMode::Hotspot;
        }else{
            ret=WifiMode::Client;
        }
    }
    return ret;
}

void WifiWrapper::setOperationMode(WifiWrapper::WifiMode p_operationMode)
{
    if(itemsAccessable()){
        // accesspoint mode is only possible if hardware if capable
        if(WifiMode::Hotspot==p_operationMode && (getWifiDevice()->wirelessCapabilities() & NetworkManager::WirelessDevice::ApCap)){
            m_currentConnection=m_accessPointCon;
            QDBusPendingReply<NMVariantMapMap> map=m_accessPointCon->secrets("802-11-wireless-security");
            m_apPassword=map.value()["802-11-wireless-security"]["psk"].toString();
            setNewSettings(true);
            reconnectSignals();
        }else if(WifiMode::Client==p_operationMode){
            m_currentConnection=m_clientCon;
            QDBusPendingReply<NMVariantMapMap> map=m_clientCon->secrets("802-11-wireless-security");
            m_password=map.value()["802-11-wireless-security"]["psk"].toString();
            setNewSettings(true);
            reconnectSignals();

            refresh();
        }

    }
    emit OperationModeChanged();
    emit ApNameChanged();
}

void WifiWrapper::setConnect(bool p_active)
{
    if(itemsAccessable()){
        m_currentDevice->setAutoconnect(p_active);
        if(!p_active){
            if(m_currentDevice->activeConnection()!=nullptr){
                NetworkManager::deactivateConnection(m_currentDevice->activeConnection()->path());
            }
        }else if(p_active){
            if(m_currentDevice->activeConnection()==nullptr){
                activateCurrentConnection();
            }
        }
    }
    emit ConStateChanged();
}

void WifiWrapper::refreshNetworks()
{
    getWifiDevice()->requestScan();
    emit AvailableNetworksChanged();
    refresh();
}

NetworkManager::WirelessDevice::Ptr WifiWrapper::getWifiDevice()
{
    return m_currentDevice.dynamicCast<NetworkManager::WirelessDevice>();
}

NetworkManager::WirelessSetting::Ptr WifiWrapper::getCurrentWirelessSettings()
{
    if(nullptr==getCurrentSetting())return nullptr;
    return getCurrentSetting()->setting(NetworkManager::Setting::Wireless).staticCast<NetworkManager::WirelessSetting>();
}

NetworkManager::WirelessSecuritySetting::Ptr WifiWrapper::getCurrentWirelessSecuritySettings()
{
    if(nullptr==getCurrentSetting())return nullptr;
    return getCurrentSetting()->setting(NetworkManager::Setting::WirelessSecurity).staticCast<NetworkManager::WirelessSecuritySetting>();
}

NetworkManager::ConnectionSettings::Ptr WifiWrapper::getCurrentApSetting()
{
    if(nullptr==m_accessPointCon)return nullptr;
    return m_accessPointCon->settings();
}

NetworkManager::ConnectionSettings::Ptr WifiWrapper::getCurrentClientSetting()
{
    if(nullptr==m_clientCon)return nullptr;
    return m_clientCon->settings();
}

NetworkManager::WirelessSetting::Ptr WifiWrapper::getCurrentWirelessApSettings()
{
    if(nullptr==getCurrentApSetting())return nullptr;
    return getCurrentApSetting()->setting(NetworkManager::Setting::Wireless).staticCast<NetworkManager::WirelessSetting>();
}

NetworkManager::WirelessSecuritySetting::Ptr WifiWrapper::getCurrentWirelessSecurityApSettings()
{
    if(nullptr==getCurrentApSetting())return nullptr;
    return getCurrentApSetting()->setting(NetworkManager::Setting::WirelessSecurity).staticCast<NetworkManager::WirelessSecuritySetting>();
}

NetworkManager::WirelessSetting::Ptr WifiWrapper::getCurrentWirelessClientSettings()
{
    if(nullptr==getCurrentClientSetting())return nullptr;
    return getCurrentClientSetting()->setting(NetworkManager::Setting::Wireless).staticCast<NetworkManager::WirelessSetting>();
}

NetworkManager::WirelessSecuritySetting::Ptr WifiWrapper::getCurrentWirelessSecurityClientSettings()
{
    if(nullptr==getCurrentClientSetting())return nullptr;
    return getCurrentClientSetting()->setting(NetworkManager::Setting::WirelessSecurity).staticCast<NetworkManager::WirelessSecuritySetting>();
}

void WifiWrapper::AccessPointAppeared(){
    emit AvailableNetworksChanged();
    refreshNetworks();
}


