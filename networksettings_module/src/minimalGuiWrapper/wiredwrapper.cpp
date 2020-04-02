#include "wiredwrapper.h"
#include <iostream>



WiredWrapper::WiredWrapper(QObject *parent): AbstractNetworkWrapper(parent), m_autoCon(nullptr), m_manuellCon(nullptr)
{
    connect(this, &AbstractNetworkWrapper::initalized,this, &WiredWrapper::init);
}

void WiredWrapper::init()
{
    setAppState(AppletStatus::Initalize);

    m_techList.clear();
    for(NetworkManager::Device::Ptr dev : m_list) {
        if(NM_DEVICE_TYPE_ETHERNET == static_cast<NMDeviceType>(dev->type())){
            m_techList.append(dev);
        }
    }
    if(m_techList.size()>0){
        m_currentDevice=m_techList[0];
    }

    //check autoConnection and create if not available
    reinit();
    emit DeviceListChanged();
    activateCurrentConnection();
    setAppState(AppletStatus::Input);

}

void WiredWrapper::reinit()
{
    if(m_currentDevice!=nullptr){
        NetworkManager::ActiveConnection::Ptr activeCon;
        activeCon=m_currentDevice->activeConnection();

        QString autoPath="";
        for(NetworkManager::Connection::Ptr connection : NetworkManager::listConnections()){
            if(connection->name() == getExpectedNameAuto()){
                autoPath=connection->path();
            }
        }
        if(""==autoPath){
            autoPath  = addConnection(getDefaultEthernetConnectionAuto()).value().path();
        }

        m_autoCon=NetworkManager::findConnection(autoPath);

        //check manualConnection and create if not avialable
        QString manualPath="";
        for(NetworkManager::Connection::Ptr connection : NetworkManager::listConnections()){
            if(connection->name() == getExpectedNameManual()){
                manualPath=connection->path();
            }
        }

        if(""==manualPath){
            manualPath  = addConnection(getDefaultEthernetConnectionManual()).value().path();
        }

        m_manuellCon=NetworkManager::findConnection(manualPath);

        //choose first connection
        if(nullptr != activeCon){
            if(activeCon->uuid()==m_manuellCon->uuid()){
                m_currentConnection=m_manuellCon;
            }else{
                m_currentConnection=m_autoCon;
            }
        }else{
            m_currentConnection=m_autoCon;
        }

        reconnectSignals();
        assumeConState();
    }
    refresh();


}

void WiredWrapper::reconnectSignals()
{
    if(m_currentDevice==nullptr)return;
    if(m_currentConnection==nullptr)return;
    for(QMetaObject::Connection qtcon : qtConnections){
        disconnect(qtcon);
    }
    qtConnections.clear();
    qtConnections.push_back(connect(m_currentConnection.data(),&NetworkManager::Connection::updated,this,&WiredWrapper::updated));
    qtConnections.push_back(connect(m_currentDevice.data(),&NetworkManager::Device::stateChanged,this,&AbstractNetworkWrapper::DeviceConStateChanged));
    qtConnections.push_back(connect(m_currentDevice.data(),&NetworkManager::Device::availableConnectionDisappeared,this,&WiredWrapper::availableConnectionDisappeared));
    qtConnections.push_back(connect(m_currentDevice->activeConnection().data(),&NetworkManager::ActiveConnection::stateChanged,this,&AbstractNetworkWrapper::ActiveConStateChanged));
}

void WiredWrapper::refresh()
{
    emit Ipv4ActiveChanged();
    emit Ipv6ActiveChanged();
    emit Ipv4AddressChanged();
    emit Ipv6AddressChanged();
    emit SubnetmaskChanged();
    emit PrefixChanged();
//    emit DeviceListChanged();
    emit CurrentDeviceChanged();
    emit CurrentIpv4ConTypeChanged();
    emit IsConnectedChanged();
}

void WiredWrapper::setCurrentIpv4ConType(ConMethod p_type)
{

    if(itemsAccessable()){
        if(ConMethod::Automatic==p_type){
            if(getCurrentIpv6ConType()==ConMethod::Automatic){
                m_currentConnection=m_autoCon;
            }else{
                m_currentConnection=m_manuellCon;
            }
            getCurrentIpv4Settings()->setMethod(NetworkManager::Ipv4Setting::Automatic);
        }else if(ConMethod::Manual==p_type){
            m_currentConnection=m_manuellCon;
            getCurrentIpv4Settings()->setMethod(NetworkManager::Ipv4Setting::Manual);
        }
        reconnectSignals();
        emit Ipv4AddressChanged();
        emit SubnetmaskChanged();
        setNewSettings(true);
    }
    emit CurrentIpv4ConTypeChanged();
}

void WiredWrapper::setCurrentIpv6ConType(ConMethod p_type)
{
    if(itemsAccessable()){
        if(ConMethod::Automatic==p_type){
            if(getCurrentIpv4ConType()==ConMethod::Automatic){
                m_currentConnection=m_autoCon;
            }else{
                m_currentConnection=m_manuellCon;
            }
            getCurrentIpv6Settings()->setMethod(NetworkManager::Ipv6Setting::Automatic);
        }else if(ConMethod::Manual==p_type){
            m_currentConnection=m_manuellCon;
            getCurrentIpv6Settings()->setMethod(NetworkManager::Ipv6Setting::Manual);
        }
        reconnectSignals();
        emit Ipv6AddressChanged();
        emit SubnetmaskChanged();
        setNewSettings(true);
    }
    emit CurrentIpv6ConTypeChanged();
}

NetworkManager::WiredDevice::Ptr WiredWrapper::getEthernetDevice()
{
    return m_currentDevice.dynamicCast<NetworkManager::WiredDevice>();
}

NetworkManager::Connection::Ptr WiredWrapper::getManuellCon()
{
    return m_manuellCon;
}

NetworkManager::Connection::Ptr WiredWrapper::getAutoCon()
{
    return m_autoCon;
}

QString WiredWrapper::getExpectedNameAuto()
{
    return "EthernetConnectionAuto";
}



QString WiredWrapper::getExpectedNameManual()
{
    return "EthernetConnectionManual";
}

NMVariantMapMap WiredWrapper::getDefaultEthernetConnectionAuto()
{
    NMVariantMapMap connection;
    connection["connection"]["uuid"] = QUuid::createUuid().toString().remove('{').remove('}');
    connection["connection"]["id"] = getExpectedNameAuto();
    connection["connection"]["type"] = "802-3-ethernet";
    connection["802-3-ethernet"];
    connection["ipv4"]["method"] = "auto";
    connection["ipv6"]["method"] = "auto";
    return connection;
}

NMVariantMapMap WiredWrapper::getDefaultEthernetConnectionManual()
{
    NMVariantMapMap connection;
    connection["connection"]["uuid"] = QUuid::createUuid().toString().remove('{').remove('}');
    connection["connection"]["id"] = getExpectedNameManual();
    connection["connection"]["type"] = "802-3-ethernet";
    connection["802-3-ethernet"];
    connection["ipv4"]["method"] = "auto";
    connection["ipv6"]["method"] = "auto";
    return connection;
}



QString WiredWrapper::getIpv4DualUse()
{
    if(getCurrentIpv4ConType()==ConMethod::Manual){
        return getIpV4Address();
    }else{
        return getActiveIpv4Ip();
    }
}

QString WiredWrapper::getIpv6DualUse()
{
    if(getCurrentIpv6ConType()==ConMethod::Manual){
        return getIpV6Address();
    }else{
        return getActiveIpv6Ip();
    }
}

QString WiredWrapper::getSubnetMaskDualUse()
{
    if(getCurrentIpv4ConType()==ConMethod::Manual){
        return getSubnetMask();
    }else{
        return getActiveSubnetmask();
    }
}

QString WiredWrapper::getPrefixDualUse()
{
    if(getCurrentIpv6ConType()==ConMethod::Manual){
        return getPrefix();
    }else{
        return getActivePrefix();
    }
}

void WiredWrapper::availableConnectionDisappeared(const QString &connection)
{
    if(connection==m_autoCon->path()){
        init();
        return;
    }
    if(connection==m_manuellCon->path()){
        init();
        return;
    }
}



