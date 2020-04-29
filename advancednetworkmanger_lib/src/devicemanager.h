#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QList>
#include <NetworkManagerQt/Device>

class DeviceManager
{
public:
    QList<QString> getDevices(NetworkManager::Device::Type p_type);
    QList<QString> getDevices();
    NetworkManager::Device::Ptr getDevice(QString p_devicePath);

private:
    QMap<QString,NetworkManager::Device::Ptr> m_devList;
public slots:
    void newDevice(QString p_devicePath);
    void deviceRemoved(QString p_devicePath);
signals:
    void addDevice(NetworkManager::Device::Type p_type, QString p_devicePath);
    void removeDevice(NetworkManager::Device::Type p_type, QString p_devicePath);
};

#endif // DEVICEMANAGER_H
