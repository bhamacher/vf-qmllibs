#ifndef WIRELESSCONNECTIONSETTINGSINTERFACE_H
#define WIRELESSCONNECTIONSETTINGSINTERFACE_H

#include <QObject>
#include "abstractconnectionsettingsinterface.h"

class WirelessConnectionSettingsInterface : public AbstractConnectionSettingsInterface
{
public:
    WirelessConnectionSettingsInterface(QObject *parent = nullptr);
};

#endif // WIRELESSCONNECTIONSETTINGSINTERFACE_H
