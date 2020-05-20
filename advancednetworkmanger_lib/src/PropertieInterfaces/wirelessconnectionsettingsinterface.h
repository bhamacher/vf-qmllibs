#ifndef WIRELESSCONNECTIONSETTINGSINTERFACE_H
#define WIRELESSCONNECTIONSETTINGSINTERFACE_H

#include <QObject>
#include "abstractconnectionsettingsinterface.h"
#include <NetworkManagerQt/WirelessSecuritySetting>

class WirelessConnectionSettingsInterface : public AbstractConnectionSettingsInterface
{
    Q_OBJECT

    Q_PROPERTY(QString ssid READ getSsid WRITE setSsid NOTIFY ssidChanged)
    Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString mode READ getMode WRITE setMode NOTIFY modeChanged)
public:
    WirelessConnectionSettingsInterface(QObject *parent = nullptr);

    Q_INVOKABLE void create() override;


    QString getSsid();
    void setSsid(QString p_ssid);

    QString getPassword();
    void setPassword(QString p_password);

    QString getMode();
    void setMode(QString p_mode);

signals:

    void ssidChanged();
    void passwordChanged();
    void modeChanged();

};

#endif // WIRELESSCONNECTIONSETTINGSINTERFACE_H
