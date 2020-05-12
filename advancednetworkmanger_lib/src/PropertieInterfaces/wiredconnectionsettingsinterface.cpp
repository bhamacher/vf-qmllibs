#include "wiredconnectionsettingsinterface.h"

WiredConnectionSettingsInterface::WiredConnectionSettingsInterface(QObject* parent) : AbstractConnectionSettingsInterface(parent)
{

}

QString WiredConnectionSettingsInterface::getIpv4Mode()
{

 QString ret = "";
 NetworkManager::Ipv4Setting::Ptr set = m_connection->settings()->setting(NetworkManager::Setting::SettingType::Ipv4).staticCast<NetworkManager::Ipv4Setting>();
 if(set != NULL){
    switch(set->method()){
    case NetworkManager::Ipv4Setting::ConfigMethod::Manual:
        ret= "MANUAL";
        break;
    case NetworkManager::Ipv4Setting::ConfigMethod::Automatic:
        ret= "AUTOMATIC";
        break;
    case NetworkManager::Ipv4Setting::ConfigMethod::Shared:
        ret= "";
        break;
    case NetworkManager::Ipv4Setting::ConfigMethod::Disabled:
        ret= "";
        break;
    case NetworkManager::Ipv4Setting::ConfigMethod::LinkLocal:
        ret= "";
        break;
    }
 }

return ret;
}

void WiredConnectionSettingsInterface::setIpv4Mode(QString p_ipv4Mode)
{

}

QString WiredConnectionSettingsInterface::getIpv4()
{

}

void WiredConnectionSettingsInterface::setIpv4(QString p_ipv4)
{

}

QString WiredConnectionSettingsInterface::getIpv4Sub()
{

}

void WiredConnectionSettingsInterface::setIpv4Sub(QString p_ipv4Sub)
{

}

QString WiredConnectionSettingsInterface::getIpv6Mode()
{

}

void WiredConnectionSettingsInterface::setIpv6Mode(QString p_ipv6Mode)
{

}

QString WiredConnectionSettingsInterface::getIpv6()
{

}

void WiredConnectionSettingsInterface::setIpv6(QString p_ipv6)
{

}

QString WiredConnectionSettingsInterface::getIpv6Sub()
{

}

void WiredConnectionSettingsInterface::setIpv6Sub(QString p_ipv6Sub)
{

}
