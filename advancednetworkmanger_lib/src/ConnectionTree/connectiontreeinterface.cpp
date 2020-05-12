#include "connectiontreeinterface.h"
#include "bluetoothnetworks.h"
#include "vpnnetworks.h"
#include "wifinetworks.h"
#include "ethernetnetworks.h"
#include "devicemanager.h"

#include <iostream>

ConnectionTreeInterface::ConnectionTreeInterface(QObject* parent) : QObject(parent)
{
    init();
}

void ConnectionTreeInterface::init()
{
    m_devManager=new DeviceManager();
    m_model = new ConnectionModel();
    ConnectionList* list =new ConnectionList();
    m_model->setList(list);
    m_networkTypeList.append(new EthernetNetworks());
    m_networkTypeList.append(new WifiNetworks());

    m_devManager->init();

    for(auto it=m_networkTypeList.begin(); it != m_networkTypeList.end(); ++it){
        (*it)->init(*list,*m_devManager);
    }

}

void ConnectionTreeInterface::removeConnection(QString p_path)
{
    NetworkManager::Connection::Ptr con = NetworkManager::findConnection(p_path);
    if(con != NULL){
        con->remove();
    }
}

QList<QString> ConnectionTreeInterface::getDevices(int p_type)
{
    if(p_type==(int)ConType::Wifi){
        return m_devManager->getDevices(NetworkManager::Device::Type::Wifi);
    }else if(p_type==(int)ConType::Cable){
        return m_devManager->getDevices(NetworkManager::Device::Type::Ethernet);
    }
    return m_devManager->getDevices(NetworkManager::Device::Type::UnknownType);
}

void ConnectionTreeInterface::connect(QString p_conPath, QString p_devPath)
{
    NetworkManager::activateConnection(p_conPath,p_devPath,"");
}

void ConnectionTreeInterface::disconnect(QString p_conPath)
{
    for(NetworkManager::ActiveConnection::Ptr acon : NetworkManager::activeConnections()){
        if(acon->connection()->path() == p_conPath){
            NetworkManager::deactivateConnection(acon->path());
            break;
        }
    }

}


QAbstractListModel* ConnectionTreeInterface::getDataListQml() const
{

return m_model;
}

