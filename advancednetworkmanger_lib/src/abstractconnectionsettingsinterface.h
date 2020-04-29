#ifndef ABSTRACTCONNECTIONSETTINGSINTERFACE_H
#define ABSTRACTCONNECTIONSETTINGSINTERFACE_H

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

class AbstractConnectionSettingsInterface : public QObject
{
     Q_OBJECT
public:
    AbstractConnectionSettingsInterface(QObject *parent = nullptr);
};

#endif // ABSTRACTCONNECTIONSETTINGSINTERFACE_H
