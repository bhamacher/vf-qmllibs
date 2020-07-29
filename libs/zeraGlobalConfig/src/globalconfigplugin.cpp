#include "globalconfigplugin.h"
#include <QCoreApplication>
#include <QtQml/QtQml>
#include "3rdparty/JsonSettingsQML/jsonglobalsettings.h"
#include "3rdparty/JsonSettingsQML/jsonsettingsfile.h"


void GlobalConfigPlugin::init()
{
    GlobalConfigPlugin* obj= new GlobalConfigPlugin();
    obj->registerTypes("GlobalConfig");
}



void GlobalConfigPlugin::registerTypes(const char* uri) {
    qmlRegisterSingletonType(QUrl("qrc:/src/qml/GlobalConfig.qml"), uri, 1, 0, "GC");
    qmlRegisterType<JsonSettingsFile>("ZeraSettings", 1, 0, "ZeraSettings");
    qmlRegisterType<JsonGlobalSettings>("ZeraSettings", 1, 0, "ZeraGlobalSettings");
}


static void init(){
    GlobalConfigPlugin::init();
}
Q_COREAPP_STARTUP_FUNCTION(init)
