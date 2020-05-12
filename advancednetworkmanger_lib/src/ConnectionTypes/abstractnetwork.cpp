#include "abstractnetwork.h"

AbstractNetwork::AbstractNetwork()
{



}

void AbstractNetwork::connectionActivated(const QString &p_path)
{
    NetworkManager::ActiveConnection::Ptr acon = NetworkManager::findActiveConnection(p_path);
    QString path = acon->connection()->path();
    if(m_conList.contains(path)){
        connectionItem itm = m_list->itemByPath(path);
        m_aConList[p_path]=path;

        if(itm.NmPath == path){
            itm.Connected=true;
            m_list->setItemByPath(path,itm);
        }
    }
}

void AbstractNetwork::connectionDeactivate(const QString &p_path)
{
    if(m_aConList.contains(p_path)){
        if(m_conList.contains(m_aConList[p_path])){
                QString path = m_aConList[p_path];
                m_aConList.remove(p_path);
                connectionItem itm = m_list->itemByPath(path);
                itm.Connected=false;
                m_list->setItemByPath(path,itm);
        }

    }
}

void AbstractNetwork::addConnectionToList(NetworkManager::Connection::Ptr p_con)
{
    bool newData=true;
    QString path=p_con->path();
    NetworkManager::Setting::Ptr set = p_con->settings()->setting(m_setType);
    if(m_conList.contains(path)){
        connectionItem con;
        con = m_list->itemByPath(path);
        con.Available=true;
        m_list->setItemByPath(path,con);
    }else{
        ConStruct conBuf;
        conBuf.con.setValue(p_con);
        if(m_conList.contains(p_con->path())){
            newData = false;
        }
        m_conList[p_con->path()]=conBuf;
        if(newData){
            m_list->addItem(CreateConItem(p_con));
        }
    }
}

void AbstractNetwork::findAvailableConnections(QString &p_uni)
{
    for(NetworkManager::Connection::Ptr connection : m_devManager->getDevice(p_uni)->availableConnections()){
        addConnectionToList(connection);
    }
}

void AbstractNetwork::findStoredConnections()
{
    bool newData=true;
    for(NetworkManager::Connection::Ptr connection : NetworkManager::listConnections()){
        newData=true;

        NetworkManager::Setting::Ptr set = connection->settings()->setting(m_setType);
        if(set != NULL){
            ConStruct conBuf;
            conBuf.con.setValue(connection);
            if(m_conList.contains(connection->path())){
                newData = false;
            }
            m_conList[connection->path()]=conBuf;
            if(newData){
                m_list->addItem(CreateConItem(connection));
            }
        }
    }
}

connectionItem AbstractNetwork::CreateConItem(NetworkManager::Connection::Ptr p_con)
{
    connectionItem item;
    return item;
}

bool AbstractNetwork::isConnectionActive(QString p_path)
{
    bool active = false;

    NetworkManager::ActiveConnection::List aConList = NetworkManager::activeConnections();
    for(NetworkManager::ActiveConnection::Ptr aCon : aConList){
        if(aCon->connection()->path() == p_path){
            m_aConList[aCon->path()]=p_path;
            active = true;
            break;
        }
    }

    return active;

}

void AbstractNetwork::addConnection(const QString &p_path)
{
    NetworkManager::Connection::Ptr connection = NetworkManager::findConnection(p_path);
    if(connection != nullptr){
        NetworkManager::Setting::Ptr set = connection->settings()->setting(m_setType);
        if(set != NULL){
            addConnectionToList(connection);
        }
    }
}

void AbstractNetwork::removeConnection(const QString &p_uni)
{
    if(m_conList.contains(p_uni)){
        for(QMetaObject::Connection qtCon : m_conList[p_uni].qtCons){
            disconnect(qtCon);
        }
        m_conList.remove(p_uni);
        m_list->removeByPath(p_uni);
    }
}

void AbstractNetwork::addDevice(NetworkManager::Device::Type p_type, QString p_device)
{
    if(p_type == m_type){
        NetworkManager::Device::Ptr dev = m_devManager->getDevice(p_device);
        DevStruct device;
        device.dev=dev;
        m_devList[p_device]=device;
        device.qtCons.append(connect(device.dev.data(),&NetworkManager::Device::availableConnectionAppeared,this,&AbstractNetwork::addConnection));
        device.qtCons.append(connect(device.dev.data(),&NetworkManager::Device::availableConnectionDisappeared,this,&AbstractNetwork::removeConnection));
        findAvailableConnections(p_device);
    }
}

void AbstractNetwork::removeDevice(QString p_device)
{

}


