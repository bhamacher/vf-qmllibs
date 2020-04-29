#include "connectiontreeinterface.h"
#include "activenetworks.h"
#include "globalDefines.h"

ActiveNetworks::ActiveNetworks()
{

}

bool ActiveNetworks::init()
{
    bool ret=true;
    if(nullptr == m_conTreeInt){
        ret = false;
    }
    if(ret){
         for(NetworkManager::Device::Ptr dev : NetworkManager::networkInterfaces()) {


            if(NM_DEVICE_TYPE_WIFI == static_cast<NMDeviceType>(dev->type())){
                NetworkManager::WirelessDevice::Ptr wdev=dev.dynamicCast< NetworkManager::WirelessDevice>();
                for(QString avCon : wdev->accessPoints()){
                    NetworkManager::AccessPoint accessPoint(avCon);
                    m_conTreeInt->addActiveConnection(accessPoint.ssid());
                }
            }

            for(NetworkManager::Connection::Ptr connection : dev->availableConnections()){
           m_conTreeInt->addStoredConnection(connection->uuid(),connection,ConSource::available);
            }

          }


    }
    return ret;
}

bool ActiveNetworks::reinit()
{

}

ConnectionTreeInterface *ActiveNetworks::getConTreeInt() const
{
    return m_conTreeInt;
}

void ActiveNetworks::setConTreeInt(ConnectionTreeInterface *conTreeInt)
{
    m_conTreeInt = conTreeInt;
}
