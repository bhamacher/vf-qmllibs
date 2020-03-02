#ifndef WIREDWRAPPER_H
#define WIREDWRAPPER_H

#include "abstractnetworkwrapper.h"
#include <NetworkManagerQt/WiredDevice>

class WiredWrapper: public AbstractNetworkWrapper
{
    Q_OBJECT
    Q_PROPERTY(QString ipv4 READ getIpv4DualUse WRITE setIpV4Address NOTIFY Ipv4AddressChanged)
    Q_PROPERTY(QString ipv6 READ getIpv6DualUse WRITE setIpV6Address NOTIFY Ipv6AddressChanged)
    Q_PROPERTY(QString subnetmask READ getSubnetMaskDualUse WRITE setSubnetMask NOTIFY SubnetmaskChanged)
    Q_PROPERTY(QString prefix READ getPrefixDualUse WRITE setPrefix NOTIFY PrefixChanged)

public:
    WiredWrapper(QObject *parent = nullptr);
    void init();
    void reinit();
    void reconnectSignals();
    void refresh();
    /**
     * @brief setCurrentIpv4ConType
     * @param p_type
     */
    void setCurrentIpv4ConType(ConMethod p_type);
    /**
     * @brief setCurrentIpv6ConType
     * @param p_type
     */
    void setCurrentIpv6ConType(ConMethod p_type);

    /**
     * @brief getIpv4DualUse
     * @return
     */
    QString getIpv4DualUse();
    /**
     * @brief getIpv6DualUse
     * @return
     */
    QString getIpv6DualUse();
    /**
     * @brief getSubnetMaskDualUse
     * @return
     */
    QString getSubnetMaskDualUse();
    /**
     * @brief getPrefixDualUse
     * @return
     */
    QString getPrefixDualUse();

private:
    /**
     * @brief retrun "static_cast<WiredDevice> m_currentDevice"
     * @return device as WiredDevice
     */
    NetworkManager::WiredDevice::Ptr  getEthernetDevice();
    /**
     * @return m_manuellCon
     */
    NetworkManager::Connection::Ptr getManuellCon();
    /**
     * @return m_autoCon
     */
    NetworkManager::Connection::Ptr getAutoCon();

    /**
     * @return "EthernetConnectionAuto"
     */
    QString getExpectedNameAuto();
    /**
     * @return "EthernetConnectionManual"
     */
    QString getExpectedNameManual();
    /**
     * @return default connection settings map
     */
    NMVariantMapMap getDefaultEthernetConnectionAuto();
    /**
     * @return default connection settings map
     */
    NMVariantMapMap getDefaultEthernetConnectionManual();



    NetworkManager::Connection::Ptr m_autoCon;
    NetworkManager::Connection::Ptr m_manuellCon;

public slots:
    void availableConnectionDisappeared(const QString &connection);



};

#endif // WIREDWRAPPER_H
