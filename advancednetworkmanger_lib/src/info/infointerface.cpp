#include "infointerface.h"

InfoInterface::InfoInterface()
{
    connect(NetworkManager::notifier(),&NetworkManager::Notifier::activeConnectionAdded, this, &InfoInterface::addActiveConnection);
    connect(NetworkManager::notifier(),&NetworkManager::Notifier::activeConnectionRemoved, this, &InfoInterface::removeActiveConnection);
    for(NetworkManager::ActiveConnection::Ptr acon : NetworkManager::activeConnections()){






    }
}

InfoInterface::~InfoInterface()
{

}

QHash<int, QByteArray> InfoInterface::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ipv4Role] = "ipv4";
    roles[ipv6Role] = "ipv6";
    roles[subnetmaskRole] = "subnetmask";
    roles[deviceRole] = "device";
    return roles;
}

int InfoInterface::rowCount(const QModelIndex &parent) const
{
 return m_activeCons.size();
}

QVariant InfoInterface::data(const QModelIndex &index, int role) const
{
    InfoStruct itm = m_activeCons.at(index.row());
    switch(role){
    case ipv4Role:
        return itm.ipv4;
        break;
    case subnetmaskRole:
        return itm.subnetmask;
        break;
    case ipv6Role:
        return itm.ipv6;
        break;
    case deviceRole:
        return itm.device;
        break;
    }
    return QVariant();
}

void InfoInterface::addActiveConnection(const QString &p_path)
{
    NetworkManager::ActiveConnection::Ptr acon = NetworkManager::findActiveConnection(p_path);
    if(acon != NULL){
        const int index = m_activeCons.size();
        emit beginInsertRows(QModelIndex(), index, index);
        InfoStruct itm;
        itm.ipv4 = acon->ipV4Config().addresses().at(0).ip().toString();
        itm.subnetmask = acon->ipV4Config().addresses().at(0).netmask().toString();
        itm.ipv6= acon->ipV6Config().addresses().at(0).ip().toString();
        itm.device = acon->devices().at(0);
        itm.path=p_path;
        m_activeCons.append(itm);
        emit endInsertRows();
    }

}

void InfoInterface::removeActiveConnection(const QString &p_path)
{
    int idx = 0;
    for(InfoStruct itm : m_activeCons){
        if(itm.path == p_path){
           beginRemoveRows(QModelIndex(), idx, idx);
           m_activeCons.removeAt(idx);
           endRemoveRows();
           break;
        }
        idx++;
    }
}
