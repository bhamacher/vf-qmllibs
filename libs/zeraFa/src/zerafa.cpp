#include "zerafa.h"
#include <QCoreApplication>
#include <QtQml/QtQml>

void ZeraFa ::init()
{
    ZeraFa* obj= new ZeraFa();
    obj->registerTypes("ZeraFa");
}

void ZeraFa ::registerTypes(const char* uri) {
//FontAwesome Free Release 5.14.0
qmlRegisterSingletonType(QUrl("qrc:/src/qml/ZeraFa514.qml"), uri, 1, 1, "FA");
//FontAwesome Free Release unknown
qmlRegisterSingletonType(QUrl("qrc:/src/qml/ZeraFaOld.qml"), uri, 1, 0, "FA");
}


static void init(){
    ZeraFa ::init();
}
Q_COREAPP_STARTUP_FUNCTION(init)
