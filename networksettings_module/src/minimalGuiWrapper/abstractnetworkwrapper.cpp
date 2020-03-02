#include "abstractnetworkwrapper.h"
#include <NetworkManagerQt/Connection>
#include <QTimer>

#include <NetworkManagerQt/ActiveConnection>
//#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/Utils>
#include <QTextStream>




#define csettings NetworkManager::ConnectionSettings::Ptr


NetworkManager::Device::List AbstractNetworkWrapper::m_list;


AbstractNetworkWrapper::AbstractNetworkWrapper(QObject *parent) : QObject(parent), m_currentDevice(nullptr), m_currentConnection(nullptr),
    m_newSettings(false),m_appState(AppletStatus::Offline),m_conState(ConnectionStatus::Unknown)
{
    QTimer::singleShot(10, this, &AbstractNetworkWrapper::checkNetworkmanager);
}

void AbstractNetworkWrapper::init()
{
    setAppState(AppletStatus::Initalize);
    QTextStream qout(stdout, QIODevice::WriteOnly);
    m_list = NetworkManager::networkInterfaces();
    emit initalized();
}

void AbstractNetworkWrapper::reinit()
{

}

void AbstractNetworkWrapper::checkNetworkmanager()
{
    if(NetworkManager::isStartingUp()){
        QTimer::singleShot(2000, this, &AbstractNetworkWrapper::checkNetworkmanager);
    }else{
        connect(NetworkManager::notifier(),&NetworkManager::Notifier::deviceAdded,this,&AbstractNetworkWrapper::newDevice);
        connect(NetworkManager::notifier(),&NetworkManager::Notifier::deviceRemoved,this,&AbstractNetworkWrapper::newDevice);
        AbstractNetworkWrapper::init();
    }
}

void AbstractNetworkWrapper::refresh()
{


}


QDBusPendingReply< QDBusObjectPath >  AbstractNetworkWrapper::addConnection(NMVariantMapMap connection)
{
    QDBusPendingReply< QDBusObjectPath > result;
    if(getAppState()==AppletStatus::Initalize){
        result = NetworkManager::addConnection(connection);
        if(result.isError()){
        }
    }
    return result;
}

QStringList AbstractNetworkWrapper::getDeviceList()
{
    QStringList names;
    Q_FOREACH(NetworkManager::Device::Ptr dev, m_techList){
        names.append(dev->interfaceName());
    }
    return names;
}


void AbstractNetworkWrapper::setCurrentDevice(QString p_currentDevice)
{

    Q_FOREACH (NetworkManager::Device::Ptr dev, m_techList) {
        if(p_currentDevice == dev->interfaceName()){
            m_currentDevice=dev;
            reinit();
            //            refresh();
            //            setNewSettings(true);
        }
    }
}

QString AbstractNetworkWrapper::getCurrentDevice()
{
    if(nullptr==m_currentDevice)return "";
    return m_currentDevice->interfaceName();
}

AbstractNetworkWrapper::ConMethod AbstractNetworkWrapper::getCurrentIpv4ConType()
{
    ConMethod ret=ConMethod::none;

    if(nullptr==getCurrentIpv4Settings()) return ret;

    switch(getCurrentIpv4Settings()->method()){
    case NetworkManager::Ipv4Setting::Automatic:
        ret=ConMethod::Automatic;
        break;
    case NetworkManager::Ipv4Setting::LinkLocal:
        break;
    case NetworkManager::Ipv4Setting::Manual:
        ret=ConMethod::Manual;
        break;
    case NetworkManager::Ipv4Setting::Shared:
        break;
    case NetworkManager::Ipv4Setting::Disabled:
        break;
    }

    return ret;
}

void AbstractNetworkWrapper::setCurrentIpv4ConType(ConMethod p_conType)
{
    if(itemsAccessable()){

        if(ConMethod::Automatic==p_conType && getCurrentIpv4Settings()->method()!=NetworkManager::Ipv4Setting::Automatic){
            getCurrentIpv4Settings()->setMethod(NetworkManager::Ipv4Setting::Automatic);
        }
        else if(ConMethod::Manual==p_conType && getCurrentIpv4Settings()->method()!=NetworkManager::Ipv4Setting::Manual)
        {
            getCurrentIpv4Settings()->setMethod(NetworkManager::Ipv4Setting::Manual);
        }
        emit Ipv4AddressChanged();
        emit SubnetmaskChanged();
        emit CurrentIpv4ConTypeChanged();
        setNewSettings(true);
    }
}

AbstractNetworkWrapper::ConMethod AbstractNetworkWrapper::getCurrentIpv6ConType()
{
    ConMethod ret=ConMethod::none;

    if(nullptr==getCurrentIpv6Settings()) return ret;

    switch(getCurrentIpv6Settings()->method()){
    case NetworkManager::Ipv6Setting::Automatic:
        ret=ConMethod::Automatic;
        break;
    case NetworkManager::Ipv6Setting::LinkLocal:
        break;
    case NetworkManager::Ipv6Setting::Manual:
        ret=ConMethod::Manual;
        break;
    case NetworkManager::Ipv6Setting::Dhcp:
        break;
    case NetworkManager::Ipv6Setting::Ignored:
        break;
    }
    return ret;
}

void AbstractNetworkWrapper::setCurrentIpv6ConType(ConMethod p_conType)
{
    if(itemsAccessable()){
        if(ConMethod::Automatic==p_conType && getCurrentIpv6Settings()->method()!=NetworkManager::Ipv6Setting::Automatic){
            getCurrentIpv6Settings()->setMethod(NetworkManager::Ipv6Setting::Automatic);
        }
        else if(ConMethod::Automatic==p_conType && getCurrentIpv6Settings()->method()!=NetworkManager::Ipv6Setting::Manual)
        {
            getCurrentIpv6Settings()->setMethod(NetworkManager::Ipv6Setting::Manual);
        }
        emit Ipv6AddressChanged();
        emit PrefixChanged();
        emit CurrentIpv6ConTypeChanged();
        setNewSettings(true);
    }
}

void AbstractNetworkWrapper::saveCurrentSetting()
{
    if(nullptr==m_currentDevice)return;
    if(nullptr==m_currentConnection){
        return;
    }
    setAppState(AppletStatus::Updating);
    m_currentDevice->setAutoconnect(false);
    if(m_currentDevice->activeConnection()!=nullptr){
        NetworkManager::deactivateConnection(m_currentDevice->activeConnection()->path());
    }
    if(NetworkManager::findConnection(m_currentConnection->path())==nullptr){
        addConnection(m_currentConnection->settings()->toMap());
    }

    NetworkManager::ConnectionSettings::Ptr settings=getCurrentSetting();
    m_nmReply=m_currentConnection->update(settings->toMap());
    setNewSettings(false);
}

bool AbstractNetworkWrapper::itemsAccessable()
{
    bool ret=true;
    if(getAppState()!=AppletStatus::Input){
        ret=false;
    }else if(nullptr==m_currentConnection){
        ret=false;
    }else if(nullptr==m_currentDevice){
        ret=false;
    }else if(getConState()==ConnectionStatus::Disconnecting){
        ret=false;
    }else if(getConState()==ConnectionStatus::Connecting){
        ret=false;
    }
    return ret;

}

ANW::AppletStatus AbstractNetworkWrapper::getAppState()
{
    return m_appState;
}

ANW::ConnectionStatus AbstractNetworkWrapper::getConState()
{
    return m_conState;
}

QString AbstractNetworkWrapper::getPopUpMsg()
{
    QString ret= "";
    if(m_popUpMsgQue.size()>0){
        ret=m_popUpMsgQue.dequeue();
    }

    return ret;
}

void AbstractNetworkWrapper::activateCurrentConnection()
{
    if(nullptr==m_currentDevice)return;
    if(nullptr==m_currentConnection)return;
    m_currentDevice->setAutoconnect(false);
    bool good=true;
    /*
    Q_FOREACH(NetworkManager::Connection::Ptr con, m_currentDevice->availableConnections()){
        if(con->path()==m_currentConnection->path()){
            good=true;
            break;
        }else{
            good=false;
        }
    }
*/
    Q_FOREACH(NetworkManager::ActiveConnection::Ptr acon,NetworkManager::activeConnections()){
        if(m_currentConnection->path()==acon->connection()->path()){
            good=false;
            break;
        }
    }

    if(good){
        QDBusPendingReply< QDBusObjectPath > result = NetworkManager::activateConnection(m_currentConnection->path(),m_currentDevice->uni(),"");
        if(result.isError()){
        }
    }
    m_currentDevice->setAutoconnect(true);
}


QString AbstractNetworkWrapper::getIpV4Address()
{
    if(nullptr==getCurrentIpv4Settings())return noAccess;
    if(getCurrentIpv4Settings()->addressData().size()==0) return noAccess;
    return getCurrentIpv4Settings()->addressData()[0]["address"].toString();
}


void AbstractNetworkWrapper::setIpV4Address(QString p_ipv4Address)
{
    if(itemsAccessable()){
        if(nullptr==getCurrentIpv4Settings())return;
        if(getCurrentIpv4Settings()->method() == NetworkManager::Ipv4Setting::Automatic)return;
        NMVariantMapList addressData=getCurrentIpv4Settings()->addressData();
        if(addressData.size()==0){
            addressData.append(QVariantMap());
        }
        addressData[0]["address"]=p_ipv4Address;
        QList< NetworkManager::IpAddress > addresses=getCurrentIpv4Settings()->addresses();
        if(addresses.size()==0){
            addresses.append(NetworkManager::IpAddress());
        }
        addresses[0].setIp(QHostAddress(p_ipv4Address));
        getCurrentIpv4Settings()->setAddressData(addressData);
        getCurrentIpv4Settings()->setAddresses(addresses);
        setNewSettings(true);
    }
    emit Ipv4AddressChanged();
}

QString AbstractNetworkWrapper::getSubnetMask()
{
    if(nullptr==getCurrentIpv4Settings())return noAccess;
    if(getCurrentIpv4Settings()->addressData().size()==0) return "N/A";
    QNetworkAddressEntry formatAdapter;
    formatAdapter.setIp(QHostAddress(getCurrentIpv4Settings()->addressData()[0]["address"].toString()));
    formatAdapter.setPrefixLength(getCurrentIpv4Settings()->addressData()[0]["prefix"].toInt());
    return formatAdapter.netmask().toString();

}

void AbstractNetworkWrapper::setSubnetMask(QString p_subnetmask)
{
    if(itemsAccessable()){
        if(getCurrentIpv4Settings()->method() == NetworkManager::Ipv4Setting::Automatic)return;
        QNetworkAddressEntry formatAdapter;
        formatAdapter.setIp(QHostAddress(getCurrentIpv4Settings()->addressData()[0]["address"].toString()));
        formatAdapter.setNetmask(QHostAddress(p_subnetmask));
        NMVariantMapList addressData=getCurrentIpv4Settings()->addressData();
        if(addressData.size()==0){
            addressData.append(QVariantMap());
        }
        addressData[0]["prefix"]=formatAdapter.prefixLength();
        QList< NetworkManager::IpAddress > addresses=getCurrentIpv4Settings()->addresses();
        if(addresses.size()==0){
            addresses.append(NetworkManager::IpAddress());
        }
        addresses[0].setNetmask(QHostAddress(p_subnetmask));
        getCurrentIpv4Settings()->setAddressData(addressData);
        getCurrentIpv4Settings()->setAddresses(addresses);
        setNewSettings(true);
    }
    emit SubnetmaskChanged();
}

QString AbstractNetworkWrapper::getIpV6Address()
{
    if(nullptr==getCurrentIpv4Settings())return noAccess;
    if(getCurrentIpv6Settings()->addressData().size()==0) return noAccess;
    return getCurrentIpv6Settings()->addressData()[0]["address"].toString();
}

void AbstractNetworkWrapper::setIpV6Address(QString p_ipv6Address)
{
    if(itemsAccessable()){
        if(getCurrentIpv6Settings()->method() == NetworkManager::Ipv6Setting::Automatic)return;
        NMVariantMapList addressData=getCurrentIpv6Settings()->addressData();
        if(addressData.size()==0){
            addressData.append(QVariantMap());
        }
        addressData[0]["address"]=p_ipv6Address;
        QList< NetworkManager::IpAddress > addresses=getCurrentIpv6Settings()->addresses();
        if(addresses.size()==0){
            addresses.append(NetworkManager::IpAddress());
        }
        addresses[0].setIp(QHostAddress(p_ipv6Address));
        getCurrentIpv6Settings()->setAddressData(addressData);
        getCurrentIpv6Settings()->setAddresses(addresses);

    }
    emit Ipv6AddressChanged();
}

QString AbstractNetworkWrapper::getPrefix()
{
    if(nullptr==getCurrentIpv4Settings())return noAccess;
    if(nullptr==m_currentDevice)return noAccess;
    if(getCurrentIpv6Settings()->addressData().size()==0) return noAccess;
    return getCurrentIpv6Settings()->addressData()[0]["prefix"].toString();
}

void AbstractNetworkWrapper::setPrefix(QString p_Prefix)
{
    if(itemsAccessable()){
        if(getCurrentIpv6Settings()->method() == NetworkManager::Ipv6Setting::Automatic)return;
        NMVariantMapList addressData=getCurrentIpv6Settings()->addressData();
        if(addressData.size()==0){
            addressData.append(QVariantMap());
        }
        addressData[0]["prefix"]=p_Prefix.toInt();
        QList< NetworkManager::IpAddress > addresses=getCurrentIpv6Settings()->addresses();
        if(addresses.size()==0){
            addresses.append(NetworkManager::IpAddress());
        }
        addresses[0].setPrefixLength(p_Prefix.toInt());
        getCurrentIpv6Settings()->setAddressData(addressData);
        getCurrentIpv6Settings()->setAddresses(addresses);
        setNewSettings(true);
    }
    emit PrefixChanged();
}


void AbstractNetworkWrapper::setConnect(bool p_active)
{
    if(itemsAccessable()){
        m_currentDevice->setAutoconnect(false);
        if(!p_active){
            if(m_currentDevice->activeConnection()!=nullptr){
                NetworkManager::deactivateConnection(m_currentDevice->activeConnection()->path());
            }
        }else if(p_active){
            if(m_currentDevice->activeConnection()==nullptr){
                activateCurrentConnection();
            }
        }
    }
    emit ConStateChanged();
}



NetworkManager::ConnectionSettings::Ptr AbstractNetworkWrapper::getCurrentSetting()
{
    if(nullptr==m_currentConnection)return nullptr;
    return m_currentConnection->settings();
}

//Private functions
NetworkManager::Ipv4Setting::Ptr AbstractNetworkWrapper::getCurrentIpv4Settings()
{
    if(nullptr==getCurrentSetting())return nullptr;
    return getCurrentSetting()->setting(NetworkManager::Setting::Ipv4).staticCast<NetworkManager::Ipv4Setting>();
}

NetworkManager::Ipv6Setting::Ptr AbstractNetworkWrapper::getCurrentIpv6Settings()
{
    if(nullptr==getCurrentSetting())return nullptr;
    return getCurrentSetting()->setting(NetworkManager::Setting::Ipv6).staticCast<NetworkManager::Ipv6Setting>();
}

void AbstractNetworkWrapper::assumeConState()
{

    // store app state to go back to later
    NetworkManager::ActiveConnection::Ptr activeCon=m_currentDevice->activeConnection();
    switch (m_currentDevice->state()) {
    case NetworkManager::Device::UnknownState:
        setConState(ConnectionStatus::Disconnected);
        break;
    case NetworkManager::Device::Unmanaged:
        setConState(ConnectionStatus::Disconnected);
        break;
    case NetworkManager::Device::Unavailable:
        setConState(ConnectionStatus::Disconnected);
        break;
    case NetworkManager::Device::Disconnected:
        setConState(ConnectionStatus::Disconnected);
        break;
    case NetworkManager::Device::Activated:
        setConState(ConnectionStatus::Connected);
        break;
    case NetworkManager::Device::NeedAuth:
        setConState(ConnectionStatus::Connecting);
        break;
    case NetworkManager::Device::Deactivating:
        setConState(ConnectionStatus::Disconnecting);
        break;
    default:
        setConState(ConnectionStatus::Connecting);
    }
}


QString AbstractNetworkWrapper::getActiveIpv4Ip()
{
    if(nullptr==m_currentDevice)return "N/A";
    if(m_currentDevice->ipV4Config().addresses().size()==0) return noAccess;
    return m_currentDevice->ipV4Config().addresses()[0].ip().toString();
}


QString AbstractNetworkWrapper::getActiveIpv6Ip()
{
    if(nullptr==m_currentDevice)return "N/A";
    if(m_currentDevice->ipV6Config().addresses().size()==0) return noAccess;
    return m_currentDevice->ipV6Config().addresses()[0].ip().toString();
}


QString AbstractNetworkWrapper::getActiveSubnetmask()
{
    if(nullptr==m_currentDevice)return "N/A";
    if(m_currentDevice->ipV4Config().addresses().size()==0) return noAccess;
    return m_currentDevice->ipV4Config().addresses()[0].netmask().toString();
}



QString AbstractNetworkWrapper::getActivePrefix()
{
    if(nullptr==m_currentDevice)return "N/A";
    if(m_currentDevice->ipV6Config().addresses().size()==0) return noAccess;
    return QString::number(m_currentDevice->ipV6Config().addresses()[0].prefixLength());
}

bool AbstractNetworkWrapper::getNewSettings() const
{
    return m_newSettings;
}

void AbstractNetworkWrapper::setNewSettings(bool newSettings)
{
    m_newSettings = newSettings;
    emit NewSettingsChanged();
}

void AbstractNetworkWrapper::setAppState(AppletStatus p_state)
{
    m_appState=p_state;
}

void AbstractNetworkWrapper::setConState(ConnectionStatus p_state)
{
    m_conState=p_state;
    emit ConStateChanged();
}

void AbstractNetworkWrapper::setPopUpMsg(QString p_msg)
{
    m_popUpMsgQue.enqueue(p_msg);
    emit PopUpMsgChanged();
}



void AbstractNetworkWrapper::updated()
{
    switch(getAppState()){
    case AppletStatus::Updating:
        //activateCurrentConnection();
        if(m_nmReply.isError()){
            setPopUpMsg(QLatin1String("Apply failed \n Please try again"));
            setNewSettings(true);
        }
        m_currentDevice->setAutoconnect(true);
        setAppState(AppletStatus::Input);
        break;
    default:
        setAppState(AppletStatus::Input);
    }
    refresh();
}


void AbstractNetworkWrapper::ActiveConStateChanged(NetworkManager::ActiveConnection::State state)
{
    assumeConState();
}

void AbstractNetworkWrapper::newDevice(QString device)
{
    AbstractNetworkWrapper::init();

}


void AbstractNetworkWrapper::DeviceConStateChanged(NetworkManager::Device::State newstate, NetworkManager::Device::State oldstate, NetworkManager::Device::StateChangeReason reason)
{
    if(newstate==NetworkManager::Device::NeedAuth && (reason>7 && reason < 12)){
        setPopUpMsg(QLatin1String("Password Wrong"));
    }
    if(newstate==NetworkManager::Device::Failed){
        setPopUpMsg(QLatin1String("Connection Failed"));
    }

    assumeConState();
    refresh();
}

