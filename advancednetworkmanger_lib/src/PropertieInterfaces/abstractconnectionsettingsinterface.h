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

    Q_PROPERTY(QString conName READ getConName WRITE setConName NOTIFY conNameChanged)
public:
    AbstractConnectionSettingsInterface(QObject *parent = nullptr);
    Q_INVOKABLE void load(QString p_path);
    Q_INVOKABLE void save();
    Q_INVOKABLE void discard();

    QString getConName();
    void setConName(QString p_conName);

protected:
    NetworkManager::Connection::Ptr m_connection;

signals:
    void conNameChanged();
    void loadComplete();



};

#endif // ABSTRACTCONNECTIONSETTINGSINTERFACE_H
