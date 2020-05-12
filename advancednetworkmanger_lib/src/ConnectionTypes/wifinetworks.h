#ifndef WIFINETWORKS_H
#define WIFINETWORKS_H
#include "abstractnetwork.h"
#include <NetworkManagerQt/WirelessDevice>


enum class ConClassType{NA = 0, Con, AP};


class WifiNetworks : public AbstractNetwork
{
    Q_OBJECT
public:
    WifiNetworks();
    bool init(ConnectionList  &p_list, DeviceManager &p_devManager) override;
private:
    void findAPs(QString &p_uni);
    connectionItem CreateConItem(NetworkManager::Connection::Ptr) override;
private:
    QMap<QString,int> aplist;
public slots:
    void addAccessPoint(const QString &p_uni);
    void removeAccessPoint(const QString &p_uni);
    void addConnection(const QString &connection) override;
    void removeConnection(const QString &connection) override;
    void addDevice(NetworkManager::Device::Type p_type, QString p_device) override;
};

Q_DECLARE_METATYPE(ConClassType)

#endif // WIFINETWORKS_H
