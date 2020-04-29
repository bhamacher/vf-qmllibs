#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H

#include "connectionlist.h"
#include "connectionmodel.h"
#include "devicemanager.h"
#include "NetworkManagerQt/Connection"


class AbstractNetwork
{
public:
    AbstractNetwork();
    virtual void init(ConnectionModel &p_model, DeviceManager &p_devManager) = 0;
    ConnectionModel *getModel() const;
    void setModel(ConnectionModel *model);

protected:
    ConnectionModel* m_model;
    QMap<QString,int> m_typeSpecificMapping;
    QMap<QString,NetworkManager::Connection::Ptr> m_conList;
    DeviceManager m_devManager;
    NetworkManager::Device::Type m_type;

public slots:
    void newConnection(const QString &connection);
    void connectionRemoved(const QString &connection);
    void newDevice(NetworkManager::Device::Type p_type, QString p_device);
    void deviceRemoved(NetworkManager::Device::Type p_type, QString p_device);
};

#endif // ABSTRACTNETWORK_H
