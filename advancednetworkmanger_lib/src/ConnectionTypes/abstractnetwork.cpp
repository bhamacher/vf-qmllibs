#include "abstractnetwork.h"

AbstractNetwork::AbstractNetwork()
{

}

ConnectionModel *AbstractNetwork::getModel() const
{
    return m_model;
}

void AbstractNetwork::setModel(ConnectionModel *model)
{
    m_model = model;
}
