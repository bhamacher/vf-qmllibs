#include "helpersplugin.h"
#include <QCoreApplication>
#include <QtQml/QtQml>



void HelpersPlugin::init()
{
    HelpersPlugin* obj= new HelpersPlugin();
    obj->registerTypes("QmlHelpers");
}



void HelpersPlugin::registerTypes(const char* uri) {
    qmlRegisterType(QUrl("qrc:/src/qml/TextHelper.qml"),uri,1,0,"TextHelper");
}


static void init(){
    HelpersPlugin::init();
}
Q_COREAPP_STARTUP_FUNCTION(init)
