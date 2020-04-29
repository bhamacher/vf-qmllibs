#ifndef WIREDCONNECTIONSETTINGSINTERFACE_H
#define WIREDCONNECTIONSETTINGSINTERFACE_H

#include <QObject>
#include "abstractconnectionsettingsinterface.h"

class WiredConnectionSettingsInterface : public AbstractConnectionSettingsInterface
{
public:
    WiredConnectionSettingsInterface(QObject *parent = nullptr);
};

#endif // WIREDCONNECTIONSETTINGSINTERFACE_H
