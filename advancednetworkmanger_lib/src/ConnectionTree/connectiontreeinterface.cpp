#include "connectiontreeinterface.h"
#include "activenetworks.h"
#include "storednetworks.h"
#include "conmodelitem.h"

#include <iostream>

ConnectionTreeInterface::ConnectionTreeInterface(QObject* parent) : QObject(parent)
{
    init();
}

void ConnectionTreeInterface::init()
{
    m_model = new ConnectionModel();
    m_storedNetworks = new StoredNetworks();
    m_activeNetworks = new ActiveNetworks();
    m_activeNetworks->setConTreeInt(this);
    m_storedNetworks->setConTreeInt(this);
    m_storedNetworks->init();
    m_activeNetworks->init();
}

void ConnectionTreeInterface::addStoredConnection(QString p_uni, NetworkManager::Connection::Ptr p_connection, ConSource p_source)
{
    if(p_connection->name() != "generic"){
    ConModelItem* dataobj= new ConModelItem();
    dataobj->setName(p_connection->name());
    dataobj->setNmPath(p_connection->path());

    p_connection->settings()->settings();
    dataobj->setGroupe("ETHERNET");
    bool fin = false;
    for (NetworkManager::Setting::Ptr set : p_connection->settings()->settings()){
        if(fin)break;
        NetworkManager::Setting::SettingType t =set->type();
        switch(t){
        case NetworkManager::Setting::SettingType::Wired:
            dataobj->setGroupe("ETHERNET");
            dataobj->setType(ConModelItem::TransType::Cable);
            fin = true;
            break;
        case NetworkManager::Setting::SettingType::Wireless:


            dataobj->setName(set.dynamicCast<NetworkManager::WirelessSetting>()->ssid());
            if(set.dynamicCast<NetworkManager::WirelessSetting>()->mode() == NetworkManager::WirelessSetting::NetworkMode::Infrastructure){
                dataobj->setGroupe("WIFI");
            }else if(set.dynamicCast<NetworkManager::WirelessSetting>()->mode() == NetworkManager::WirelessSetting::NetworkMode::Ap){
                dataobj->setGroupe("HOTSPOT");
            }else{
                dataobj->setGroupe("UNKNOWN");
            }

            p_uni=dataobj->getName();
            dataobj->setType(ConModelItem::TransType::Wireless);
            fin = true;
            break;
        case NetworkManager::Setting::SettingType::Vpn:
            dataobj->setGroupe("VPN");
            dataobj->setType(ConModelItem::TransType::Cable);
            fin = true;
            break;
        default:
            dataobj->setGroupe("UNKNOWN");
        }
    }

    if(p_source == ConSource::available){
        dataobj->setAvailable(true);
    }else{
        dataobj->setAvailable(false);
    }
    //dataobj->setAvailable(false);
    dataobj->setConnected(false);
    dataobj->setSignalStrength(100);

    //if(dataobj->getGroupe() != "UNKNOWN"){
    m_model->addData(p_uni,dataobj);
    emit dataListChanged();
    //}else{
       // delete  dataobj;
    }
    }

void ConnectionTreeInterface::addActiveConnection(QString name)
{
    ConModelItem* dataobj= new ConModelItem();
    dataobj->setName(name);
    dataobj->setNmPath("");
    dataobj->setGroupe("WIFI");
    dataobj->setAvailable(true);
    dataobj->setConnected(false);
    dataobj->setSignalStrength(100);
    m_model->addData(name,dataobj);
    emit dataListChanged();
}



void ConnectionTreeInterface::removeConnection(QString p_uni)
{

}

//QList<QObject *> ConnectionTreeInterface::getDataList() const
//{
//    return m_dataList.values();

//}
QAbstractListModel* ConnectionTreeInterface::getDataListQml() const
{

return m_model;
}

