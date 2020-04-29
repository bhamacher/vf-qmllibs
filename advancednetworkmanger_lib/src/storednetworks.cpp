#include "connectiontreeinterface.h"
#include "storednetworks.h"

#include "globalDefines.h"

StoredNetworks::StoredNetworks()
{

}

bool StoredNetworks::init()
{

    bool ret=true;
    if(nullptr == m_conTreeInt){
        ret = false;
    }
    if(ret){
        for(NetworkManager::Connection::Ptr connection : NetworkManager::listConnections()){
            m_conTreeInt->addStoredConnection(connection->uuid(),connection,ConSource::stored);
        }
    }
    return ret;
}

ConnectionTreeInterface *StoredNetworks::getConTreeInt() const
{
    return m_conTreeInt;
}

void StoredNetworks::setConTreeInt(ConnectionTreeInterface *conTreeInt)
{
    m_conTreeInt = conTreeInt;
}
