#include "globalconfigplugin.h"
#include <QCoreApplication>
#include <QtQml/QtQml>
#include <jsonglobalsettings.h>
#include <jsonsettingsfile.h>


void GlobalConfigPlugin::init()
{
    GlobalConfigPlugin* obj= new GlobalConfigPlugin();
    obj->registerTypes("GlobalConfig");
}



void GlobalConfigPlugin::registerTypes(const char* uri) {
    qmlRegisterSingletonType(QUrl("qrc:/src/qml/GlobalConfig.qml"), uri, 1, 0, "GC");
}


static void init(){
    GlobalConfigPlugin::init();
}
Q_COREAPP_STARTUP_FUNCTION(init)
