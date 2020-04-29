#ifndef CONNECTIONLIST_H
#define CONNECTIONLIST_H

#include <QObject>
#include <QString>
#include <QMap>

#include "globalDefines.h"


class connectionItem{
public:
    QString Groupe;
    QString Name;
    QString NmPath;
    QString Available;
    int SignalStrength;
    ConType Type;
    bool Connected;
};



class ConnectionList : public QObject
{
public:
    ConnectionList();
    int addItem(connectionItem &Item);
    bool removeItem(int id);
    QList<connectionItem> items() const;
    bool setItemAt(int index, const connectionItem &p_item);
private:
    QMap<int,connectionItem> m_idMap;
    int m_uidCounter;

public slots:
    void appendItem();
    void removeItem();

signals:
    void preItemRemoved();
    void postItemRemoved();

    void preItemAppended();
    void postItemAppendend();

};

#endif // CONNECTIONLIST_H
