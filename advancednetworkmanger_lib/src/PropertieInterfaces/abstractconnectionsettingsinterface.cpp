#include "abstractconnectionsettingsinterface.h"

AbstractConnectionSettingsInterface::AbstractConnectionSettingsInterface(QObject* parent) : QObject(parent)
{
    m_connection = NULL;
}

void AbstractConnectionSettingsInterface::load(QString p_path)
{
    m_connection = NetworkManager::findConnection(p_path);
    m_settings = m_connection->settings();
    m_connectionMap = m_connection->settings()->toMap();
    emit loadComplete();

}

void AbstractConnectionSettingsInterface::create()
{
    m_settings= NetworkManager::ConnectionSettings::Ptr::create(NetworkManager::ConnectionSettings::ConnectionType::Unknown);

}

void AbstractConnectionSettingsInterface::save()
{
    if(m_connection != NULL){
        m_connection->update(m_settings->toMap());
    }else{
        NMVariantMapMap map = m_settings->toMap();
        NetworkManager::addConnection(map);
        m_settings.clear();
    }
}

void AbstractConnectionSettingsInterface::saveAndActivate(const QString &p_devUni)
{
    if(m_connection == NULL){
        NMVariantMapMap map = m_settings->toMap();
        NetworkManager::addAndActivateConnection(map,p_devUni,"");
        m_settings.clear();
    }
}

void AbstractConnectionSettingsInterface::discard()
{
    if(m_connection != NULL){
        m_connection->updateUnsaved(m_connectionMap);
    }else{
        NMVariantMapMap map = m_settings->toMap();
        m_settings.clear();
    }
}

QString AbstractConnectionSettingsInterface::getConName()
{

    QString ret = "";

    if(m_connection !=  NULL){
        ret = m_settings->id();
    }
    return ret;
}

void AbstractConnectionSettingsInterface::setConName(QString p_conName)
{
    m_settings->setId(p_conName);
    emit conNameChanged();
}
