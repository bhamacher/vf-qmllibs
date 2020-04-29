#ifndef CONNECTIONTREEINTERFACE_H
#define CONNECTIONTREEINTERFACE_H


#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>


#include <QLatin1String>

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>


#include <NetworkManagerQt/Setting>

#include <QtCore/QUuid>
#include <QQueue>

#include <NetworkManagerQt/Utils>

#include <iostream>

#include <globalDefines.h>
#include <connectionmodel.h>
#include <QStandardItemModel>
#include <QVector>

#include "abstractnetwork.h"

class ConModelItem;
class StoredNetworks;
class ActiveNetworks;

class ConnectionTreeInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractListModel* dataList READ getDataListQml NOTIFY dataListChanged)
public:
    ConnectionTreeInterface(QObject* paretn = nullptr);

    void init();
    void reinit();

    void addStoredConnection(QString p_uni, NetworkManager::Connection::Ptr p_connection, ConSource p_source);
    void addActiveConnection(QString name);
    void removeConnection(QString p_uni);




//    QList<QObject*> getDataList() const;
    QAbstractListModel* getDataListQml() const;


private:
    QVector<AbstractNetwork*> m_networkTypeList;

    ConnectionModel* m_model;
signals:
    void dataListChanged();

};

#endif // CONNECTIONTREEINTERFACE_H
