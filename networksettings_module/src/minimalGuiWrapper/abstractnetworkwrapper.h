#ifndef ABSTRACTNETWORKWRAPPER_H
#define ABSTRACTNETWORKWRAPPER_H

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


//some defines
#define noAccess QLatin1String("N/A")
#define ANW AbstractNetworkWrapper





/** Generic Gui Networkinterface.
 *
 *  This Class implments generic functions to implement
 *  easy to use networkapplets. Ths concept is to implement pages
 *  depending on the used hardware. The Gui always shows one predefined connection
 *  type for the device on focus currently.
 *
 *  1. Finds network devices
 *  2. Provides DBus-Gui interface to ipv4 data
 *  3. Provides DBus-Gui interface to ipv6 data
 *  4. Provides Attributes importand for all kind off network interfaces
 *
 *  holds information about:
 *  1. all networkdevices
 *  2. the technology specif devices (fill in inheriting class)
 *  3. the device in focus
 *  4. the connection(use case) in focus
 */
class AbstractNetworkWrapper: public QObject
{
    Q_OBJECT
    // List with tech specific devices
    Q_PROPERTY(QStringList deviceList READ getDeviceList NOTIFY DeviceListChanged)
    // Device in focus
    Q_PROPERTY(QString currentDevice READ getCurrentDevice WRITE setCurrentDevice NOTIFY CurrentDeviceChanged)
    // new settings to apply
    Q_PROPERTY(bool newSettings READ getNewSettings WRITE setNewSettings NOTIFY NewSettingsChanged)
    // Connection active or not
    Q_PROPERTY(ConnectionStatus conState READ getConState WRITE setConState  NOTIFY ConStateChanged)
    //the applet status
    Q_PROPERTY(AppletStatus appState READ getAppState NOTIFY AppStateChanged)
    //new popup msg
    Q_PROPERTY(QString popupMsg READ getPopUpMsg NOTIFY PopUpMsgChanged)
    //ipv4 connection method automatic/manual
    Q_PROPERTY(ConMethod currentIpv4ConType READ getCurrentIpv4ConType WRITE setCurrentIpv4ConType NOTIFY CurrentIpv4ConTypeChanged)
    //ipv6 connection method automativ/manual
    Q_PROPERTY(ConMethod currentIpv6ConType READ getCurrentIpv6ConType WRITE setCurrentIpv6ConType NOTIFY CurrentIpv6ConTypeChanged)
    //true activates the current connection/ false deactivates the current connection
    Q_PROPERTY(bool connect WRITE setConnect)

public:
    /**
     * @brief Defines the applet states
     * Offline: waiting for NM
     * Initilaize: Init or Reinit
     * AddingConnection: Adding networkconnectioN
     * Updating: Updating existing network connection
     * Busy: Waiting for NM response
     * Input: (Idle) Waiting for user input
     */
    enum class AppletStatus{Offline,Initalize,AddingConnection,Updating,Busy,Input};
    /**
     * @brief Defines the connection states
     *
     * This is an heavy reduces verison of the states
     * provided by the networkmanager
     */
    enum class ConnectionStatus{Unknown,Connecting,Disconnecting,Connected,Disconnected};
    /**
     * @brief Connectionmethods
     *
     * Automatic: DHCP
     * Manuell: Static ip
     *
     */
    enum class ConMethod{none,Automatic, Manual};

    Q_ENUM(AppletStatus)
    Q_ENUM(ConnectionStatus)
    Q_ENUM(ConMethod)

    AbstractNetworkWrapper(QObject *parent = nullptr);
    /** @brief generic init for all networdevices
     *
     *  finds all available networkdecives
     */
    virtual void init();
    virtual void reinit();

    /**
     * @brief checks if networkmanager is online
     */
    void checkNetworkmanager();
    /** @brief interface function refresh gui
     *
     *  refresh the gui.
     *  This is implemented, so this class can call the refresh function
     *  of the inherting class.
     */
    virtual void refresh();

    /** @brief Add connection to Networkmanager over DBUS
     *
     * @param[IN] connection map (documentation at: https://developer.gnome.org/NetworkManager/stable/nm-settings.html)
     * @return Networkmanager reply
     */

    QDBusPendingReply< QDBusObjectPath >  addConnection(NMVariantMapMap connection);

    /** @brief tech specific Devices by name as StringList
     *
     *  The list must be filled in the inheriting type
     *
     *  @return all devices of one speciic type
     */
    virtual QStringList getDeviceList();
    /**
     * @brief set current device by name
     * @param [IN] the device name as String. (eth0 for example)
     */
    virtual Q_INVOKABLE void setCurrentDevice(QString p_currentDevice);
    /**
     * @brief get the Name of the current device
     *
     * only possible in state ConnectionStatus::idle
     *
     * @return name as string
     */
    virtual QString getCurrentDevice();

    /**
     * @brief returns the ipv4 connection method as string
     *
     * The connection method regard the connection currently in focus.
     * Possible Types are:
     * "AUTOMATIC" = DHCP Server
     * "MANUAL" = static ip
     * @return type as string
     */
    virtual ConMethod getCurrentIpv4ConType();

    /**
     * @brief sets the ipv4 connection method
     *
     * the method is set for the connection in focus.
     * Possible Types are:
     * "AUTOMATIC" = DHCP Server
     * "MANUAL" = static ip
     *
     * undefined input will be treated as "AUTOMATIC"
     *
     * only possible in state ConnectionStatus::idle
     * @param p_type
     */
    virtual Q_INVOKABLE void setCurrentIpv4ConType(ConMethod p_type);

    /**
     * @brief returns the active connections ip address
     *
     * if there is a active connection managed by this manager or the
     * system owen applet. This fuction will return its ip.
     *
     * @return active ip as String
     */
    virtual QString getActiveIpv4Ip();
    /**
     * @brief return the ip address
     *
     * regards the connection currently in focus.
     * Active or not.
     *
     * @return ip as string
     */
    virtual QString getIpV4Address();
    /**
     * @brief set ip address
     *
     * sets the ip address for the connection currently
     * in focus
     *
     * only possible in state ConnectionStatus::idle
     *
     * @param [IN] ip address as string
     */
    virtual Q_INVOKABLE void setIpV4Address(QString p_ipv4Address);
    /**
     * @brief returns the active connections subnetmask
     *
     * if there is a active connection managed by this manager or the
     * system owen applet. This fuction will return its subnetsmask.
     *
     * @return subnetmask as String
     */
    virtual QString getActiveSubnetmask();
    /**
     * @brief return the subnetmask
     *
     * regards the connection currently in focus.
     * Active or not.
     *
     * @return subnetmask as string
     */
    virtual QString getSubnetMask();
    /**
     * @brief set subnetmask
     *
     * sets the subnetmask for the connection currently
     * in focus
     *
     * only possible in state ConnectionStatus::idle
     *
     * @param [IN] subnetmask as string
     */
    virtual Q_INVOKABLE void setSubnetMask(QString p_subnetmask);
    /**
     * @brief returns the ipv6 connection method as string
     *
     * The connection method regard the connection currently in focus.
     * Possible Types are:
     * "AUTOMATIC" = DHCP Server
     * "MANUAL" = static ip
     * @return type as string
     */
    virtual ConMethod getCurrentIpv6ConType();
    /**
     * @brief sets the ipv6 connection method
     *
     * the method is set for the connection in focus.
     * Possible Types are:
     * "AUTOMATIC" = DHCP Server
     * "MANUAL" = static ip
     *
     * undefined input will be treated as "AUTOMATIC"
     *
     * only possible in state ConnectionStatus::idle
     *
     * @param p_type
     */
    virtual Q_INVOKABLE void setCurrentIpv6ConType(ConMethod p_type);
    /**
     * @brief returns the active connections ipv6 address
     *
     * if there is a active connection managed by this manager or the
     * system owen applet. This fuction will return its ip.
     *
     * @return active ip as String
     */
    virtual QString getActiveIpv6Ip();
    /**
     * @brief return the ipv6 address
     *
     * regards the connection currently in focus.
     * Active or not.
     *
     * @return subnetmask as string
     */
    virtual QString getIpV6Address();
    /**
     * @brief set ipv6 address
     *
     * sets the ip address for the connection currently
     * in focus
     *
     * only possible in state ConnectionStatus::idle
     *
     * @param [IN] ipv6 address as string
     */
    virtual Q_INVOKABLE void setIpV6Address(QString p_ipv6Address);
    /**
     * @brief returns the active connections ipv6 prefix
     *
     * if there is a active connection managed by this manager or the
     * system owen applet. This fuction will return its prefix.
     *
     * @return prefix as String
     */
    virtual QString getActivePrefix();
    /**
     * @brief returns the prefix
     *
     * regards the connection currently in focus.
     * Active or not.
     *
     * @return prefix as string
     */
    virtual QString getPrefix();
    /**
     * @brief set prefix
     *
     * sets the prefix for the connection currently
     * in focus
     *
     * only possible in state ConnectionStatus::idle
     *
     * @param [in] ipv6 prefix as string
     */
    virtual Q_INVOKABLE void setPrefix(QString p_prefix);
    /**
     * @brief activates the connection
     *
     * This function activates the connection currently in focus
     * or deactivates the active connection depending on the status
     *
     * is is not a direct getter or setter method.
     * The name is chosen, because it is connected to qml via
     * bidirectional propertybinding over DBUS to the networkmanager.
     *
     * only possible in state ConnectionStatus::idle
     *
     * @param [in] wished outcome as bool
     */
    virtual Q_INVOKABLE void setConnect(bool p_active);

    /**
     * @brief update settings over DBUS
     *
     * All changes stored locally in this class will be send to the
     * networkmanager. Once send the updaeted connection will be activated.
     *
     * will set state to ConnectionStatus::updating.
     * The networkmanger signal updated will reset it to ConnectionStatus::idle
     *
     * This function is accessable from qml.
     */
    Q_INVOKABLE void saveCurrentSetting();
    /**
     * @brief get applet state
     *
     * @return state as enum ConnectionStatus
     */
    AppletStatus getAppState();
    /**
     * @brief getConState
     * @return
     */
    ConnectionStatus getConState();

    QString getPopUpMsg();
protected:
    /**
     * @brief returns if items are accessable
     *
     * it is only allowed to set new properties in state connected or
     * disconnected. this function returns true, if this is the case.
     *
     * additionaly it will check if the current device and connection
     * are valid. returns flas if not.
     *
     * @return true when items are accessable
     */

    bool itemsAccessable();
    /**
     * @brief make the connection currently in focus to the active connection
     */
    void activateCurrentConnection();
    /**
     * @brief are new setting to update available
     *
     * true: yes
     * false: no
     *
     * @return status as bool
     */
    bool getNewSettings() const;
    /**
     * @brief set true when new settings are available
     *        set false when update was done
     * @param status as bool
     */
    Q_INVOKABLE void setNewSettings(bool newSettings);
    /**
     * @brief set the current autmomat status
     * @param [IN] state as ConnectionStatus
     */
    Q_INVOKABLE void setAppState(AppletStatus p_state);
    /**
     * @brief setConState
     * @param p_state
     */
    Q_INVOKABLE void setConState(ConnectionStatus p_state);

    Q_INVOKABLE void setPopUpMsg(QString p_msg);

    QVector<QMetaObject::Connection> qtConnections; /*! list qt qt connection handles */
    /**
     * @brief get current connection settings
     * @return setting
     */
    NetworkManager::ConnectionSettings::Ptr getCurrentSetting();
    /**
     * @brief get current connection setting ipv4 section
     * @return Ipv4Settings
     */
    NetworkManager::Ipv4Setting::Ptr getCurrentIpv4Settings();
    /**
     * @brief get current connection setting ipv6 section
     * @return Ipv6Setting
     */
    NetworkManager::Ipv6Setting::Ptr getCurrentIpv6Settings();
    /**
     * @brief assumes the devices current connection status
     *
     * the assumption is based on the device state
     * of the device in focus and the active connection
     * information.
     */
    void assumeConState(NetworkManager::Device::State state = NetworkManager::Device::UnknownState);

protected:
    /**
     * @brief m_list
     */
    static NetworkManager::Device::List m_list; /*! List with all networkdevices */
    NetworkManager::Device::List m_techList; /*! List with devices of one type */
    NetworkManager::Device::Ptr  m_currentDevice; /*! Device in focus */
    NetworkManager::Connection::Ptr  m_currentConnection; /*! Connection in focus*/

    bool m_newSettings; /*! new settings flag*/
    QQueue<QString> m_popUpMsgQue;

    AppletStatus m_appState; /*! applet gui status */
    AppletStatus m_lastAppState; /*! applet gui status */
    ConnectionStatus m_conState; /*! applet connection status */

    QDBusPendingReply<> m_nmReply; /* Store nm rplay handle*/

signals:
    /**
     * @brief thrown when initilized
     *
     * The inheriting class can init when this signal is thrown.
     *
     */
    void initalized();
    /**
     * @brief active Ipv4 address probably changed
     */
    void Ipv4ActiveChanged();
    /**
     * @brief active Ipv6 address probably changed
     */
    void Ipv6ActiveChanged();
    /**
     * @brief setting ipv4 address probably changed
     */
    void Ipv4AddressChanged();
    /**
     * @brief setting ipv6 address probably changed
     */
    void Ipv6AddressChanged();
    /**
     * @brief SubnetmaskChanged
     */
    void SubnetmaskChanged();
    /**
     * @brief setting prefix probably changed
     */
    void PrefixChanged();
    /**
     * @brief list with networkdevices probably chnaged
     */
    void DeviceListChanged();
    /**
     * @brief device in focus probably changed
     */
    void CurrentDeviceChanged();
    /**
     * @brief Ipv4 connection method probably changed
     */
    void CurrentIpv4ConTypeChanged();
    /**
     * @brief ipv6 connection method probably changed
     */
    void CurrentIpv6ConTypeChanged();
    /**
     * @brief new settings availability changed
     */
    void NewSettingsChanged();
    /**
     * @brief Connection status changed
     */
    void IsConnectedChanged();
    /**
     * @brief Connection status chnaged
     */
    void ConStateChanged();
    /**
     * @brief applet status changed
     */
    void AppStateChanged();
    /**
     * @brief PopUpMsgChanged
     */
    void PopUpMsgChanged();
public slots:
    /**
     * @brief recives updated from networkmanager
     */
    void updated();
    /**
     * @brief recives devicedAdded/Removed from networkmanager
     * @param device
     */
    void newDevice(QString device);
    /**
     * @brief connectionStateChanged
     */
    void DeviceConStateChanged(NetworkManager::Device::State newstate, NetworkManager::Device::State oldstate, NetworkManager::Device::StateChangeReason reason);
    /**
     * @brief recives stateChanged from networkmanager
     * @param newstate
     * @param oldstate
     * @param reason
     */
    void ActiveConStateChanged(NetworkManager::ActiveConnection::State state);
};

#endif // ABSTRACTNETWORKWRAPPER_H
