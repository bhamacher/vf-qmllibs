#ifndef ACTIVENETWORKS_H
#define ACTIVENETWORKS_H


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

class ActiveNetworks : public QObject
{
public:
    ActiveNetworks();
    bool init();
    bool reinit();
    ConnectionTreeInterface *getConTreeInt() const;
    void setConTreeInt(ConnectionTreeInterface *getConTreeInt);

private:
    ConnectionTreeInterface* m_conTreeInt;
};

#endif // ACTIVENETWORKS_H
