#ifndef STOREDNETWORKS_H
#define STOREDNETWORKS_H


#include <QObject>

#include <QLatin1String>

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>


#include <NetworkManagerQt/Setting>
#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>

#include <QtCore/QUuid>
#include <QQueue>

#include <NetworkManagerQt/Utils>

#include <iostream>

class ConnectionTreeInterface;

class StoredNetworks
{
public:
    StoredNetworks();
    bool init();


    ConnectionTreeInterface *getConTreeInt() const;
    void setConTreeInt(ConnectionTreeInterface *getConTreeInt);

private:

    ConnectionTreeInterface* m_conTreeInt;
};

#endif // STOREDNETWORKS_H
