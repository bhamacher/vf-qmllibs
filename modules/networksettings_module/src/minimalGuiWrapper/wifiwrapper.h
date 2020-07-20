#ifndef WIFIWRAPPER_H
#define WIFIWRAPPER_H

#include "abstractnetworkwrapper.h"
#include <NetworkManagerQt/WirelessDevice>





/**
 * @brief Wifi Gui interface
 *
 * This class implements qml interfaces specificly needed by the wifi gui.
 *
 * Sequence:
 * init: - writing all wifi devices stored in m_list to m_techList
 *       - searching for clientConnection and AccessPointConnection
 *       - adding connections if not found
 *
 * hold information about:
 * 1. all wifi devices
 * 2. the in nm stored accesspoint connection
 * 3: the in nm stored client connection
 */


class WifiWrapper: public AbstractNetworkWrapper
{
    Q_OBJECT
    // available wireless networks as list
    Q_PROPERTY(QStringList availableNetworks READ getAvailableNetworks NOTIFY AvailableNetworksChanged)
    // the wireless network currently in focus
    Q_PROPERTY(QString currentNetwork READ getCurrentNetwork WRITE setCurrentNetwork NOTIFY CurrentNetworkChanged)
    // acesspoint name
    Q_PROPERTY(QString apName READ getApName WRITE setApName NOTIFY ApNameChanged)
    // accesspoint password
    Q_PROPERTY(QString apPassword READ getApPassword WRITE setApPassword NOTIFY ApPasswordChanged)
    // the connecetion works either as client or as access point
    Q_PROPERTY(WifiMode operationMode READ getOperationMode WRITE setOperationMode NOTIFY OperationModeChanged)
    // the passowrd to connect to a wifi network
    Q_PROPERTY(QString apLoginPassword READ getApLoginPassword WRITE setApLoginPassword NOTIFY ApLoginPasswordChanged)
    // the network this pc is currently connected to (managed by this applet ot not)
    Q_PROPERTY(QString conNetwork READ getConnectedNetwork NOTIFY ConnectedNetworkChanged)
public:

    enum class WifiMode{ModeUnknown,Client,Hotspot};
    Q_ENUM(WifiMode)

    WifiWrapper(QObject *parent = nullptr);
    ~WifiWrapper();
    /**
     * @brief reinit
     */
    void reinit();
    /**
     * @brief reconnectSignals all qt signals to devices and connections
     */
    void reconnectSignals();
    /**
     * @brief refresh gui
     */
    void refresh();

    /**
     * @brief returns list with available wifi networks
     * @return list as QStringList
     */
    QStringList getAvailableNetworks();

    /**
     * @brief returns wifi network currently in focus
     * @return network by name as string
     */
    QString getCurrentNetwork();
    /**
     * @brief set next network in focus
     *
     * Will switch to the new network if tag is valid
     *
     * @param [IN] wifi network by name as string
     */
    Q_INVOKABLE Q_INVOKABLE void setCurrentNetwork(QString p_currentNetwork);
    /**
     * @brief returns the connected network.
     *
     * The wifi network this device is part from or host of.
     *
     * @return networkname as string
     */
    QString getConnectedNetwork();
    /**
     * @brief returns wifi login password
     *
     * returns the passoword used to connect to
     * a network as client
     *
     * @return password as string
     */
    QString getApLoginPassword();
    /**
     * @brief set wifi login password
     *
     * sets the password used to connect to a
     * not open wifi as client.
     *
     * @param [IN] password as string
     */
    Q_INVOKABLE Q_INVOKABLE void setApLoginPassword(QString ApPassword);
    /**
     * @brief returns accesspoint name
     *
     * This function returns the accespoint name
     * defined by the user
     *
     * @return ap name as string
     */
    QString getApName();
    /**
     * @brief set name used for accesspoint
     * @param [IN] name as string
     */
    Q_INVOKABLE void setApName(QString ApName);
    /**
     * @brief returns the accesspoint password
     *
     * return the password neccessary to connect to this
     * device, when configured as hotspot.
     *
     * @return
     */
    QString getApPassword();
    /**
     * @brief set the accesspoint password
     * @param [IN] password as string
     */
    Q_INVOKABLE void setApPassword(QString ApPassword);
    /**
     * @brief returns operation mode
     * Modes: infrastructure: "client"
     *        hotspot: "ap"
     *
     * @return mode as string
     */
    WifiMode getOperationMode();
    /**
     * @brief set opertation mode
     * Modes: infrastructure: "client"
     *        hotspot: "ap"
     *
     * invalid modes will be treated as infrastructure.
     *
     * @param [IN] mode as string
     */
    Q_INVOKABLE void setOperationMode(WifiMode p_operationMode);

    /**
     * @brief setConnect
     * @param p_active
     */
    Q_INVOKABLE void setConnect(bool p_active);
    /**
     * @brief refresh avialable networks
     *
     * asks nm to scan for wifi networks and refresh the list.
     *
     * @param [IN] mode as string
     */
    Q_INVOKABLE void refreshNetworks();
protected:

    /**
     * @brief returns wifiDevice pointer
     * @return WifiDevice Pointer
     */
    NetworkManager::WirelessDevice::Ptr getWifiDevice();
    /**
     * @brief returns current wireless settings
     * @return current wireless settings
     */
    NetworkManager::WirelessSetting::Ptr getCurrentWirelessSettings();
    /**
     * @brief retruns current wireless security settings
     * @return
     */
    NetworkManager::WirelessSecuritySetting::Ptr getCurrentWirelessSecuritySettings();
    /**
     * @brief returns current access point settings
     * @return Connection Settings
     */
    NetworkManager::ConnectionSettings::Ptr getCurrentApSetting();
    /**
     * @brief returns current client settings
     * @return Connection Settings
     */
    NetworkManager::ConnectionSettings::Ptr getCurrentClientSetting();
    /**
     * @brief returns access point WirelessSettings
     * @return Wireless Settings
     */
    NetworkManager::WirelessSetting::Ptr getCurrentWirelessApSettings();
    /**
     * @brief retunrs acces point security settings
     * @return Wireless Security Settings
     */
    NetworkManager::WirelessSecuritySetting::Ptr getCurrentWirelessSecurityApSettings();
    /**
     * @brief returns current client Wireless Settings
     * @return Wireless Settings
     */
    NetworkManager::WirelessSetting::Ptr getCurrentWirelessClientSettings();
    /**
     * @brief returns current client Wireless Security Settings
     * @return Wireless Security Settings
     */
    NetworkManager::WirelessSecuritySetting::Ptr getCurrentWirelessSecurityClientSettings();

    /**
     * @brief returns map with default client connection settings
     * @return connection settings as VariantMapMap
     */
    NMVariantMapMap getDefaultClientConnection();
    /**
     * @brief returns map with default access point connection settings
     * @return connection settings as VariantMapMap
     */
    NMVariantMapMap getDefaultAccessPointConnection();

    /**
     * @brief returns nm name of client connection
     * @return connection name
     */
    QString getExpectedClientName();
    /**
     * @brief returns nm name of access point connection
     * @return connection name
     */
    QString getExpectedAccessPointName();

    QString m_password; /*! not in use */
    QString m_apPassword; /*! not in use*/
private:
    NetworkManager::Connection::Ptr m_clientCon; /*! pointer to client connection settings */
    NetworkManager::Connection::Ptr m_accessPointCon; /*! pointer to accesspoint connection settings */
signals:
    /**
     * @brief Wifi networks appeared or disappeared
     */
    void AvailableNetworksChanged();
    /**
     * @brief the netowrk currnetly in focus changed
     */
    void CurrentNetworkChanged();
    /**
     * @briefteh access point name changed
     */
    void ApNameChanged();
    /**
     * @brief the accesspoint password changed
     */
    void ApPasswordChanged();
    /**
     * @brief the operationmode changed
     */
    void OperationModeChanged();
    /**
     * @brief the wifi login password changed
     */
    void ApLoginPasswordChanged();
    /**
     * @brief the connection changed
     */
    void ConnectedNetworkChanged();

public slots:
    /**
     * @brief init starts when the generic init functions finishs
     */
    void init();
    /**
     * @brief connected to NM over DBUS
     */
    void AccessPointAppeared();

};

#endif // WIFIWRAPPER_H
