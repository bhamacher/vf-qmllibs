#include "abstractconnectionsettingsinterface.h"

AbstractConnectionSettingsInterface::AbstractConnectionSettingsInterface(QObject* parent) : QObject(parent)
{

}

void AbstractConnectionSettingsInterface::load(QString p_path)
{
    m_connection = NetworkManager::findConnection(p_path);

    emit loadComplete();

}

void AbstractConnectionSettingsInterface::save()
{

}

void AbstractConnectionSettingsInterface::discard()
{

}

QString AbstractConnectionSettingsInterface::getConName()
{

    QString ret;
    if(m_connection != NULL){
        ret =  m_connection->name();
    }else{
        ret = "";
    }

    return ret;
}

void AbstractConnectionSettingsInterface::setConName(QString p_conName)
{

}
