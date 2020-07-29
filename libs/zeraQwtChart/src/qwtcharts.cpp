#include "qwtcharts.h"
#include <QCoreApplication>
#include <QtQml/QtQml>
#include "fftbarchart.h"
#include "hpwbarchart.h"
#include "barchart.h"
#include "cbar.h"


void QwtCharts::init()
{
    QwtCharts* obj= new QwtCharts();
    obj->registerTypes("QwtChart");
}



void QwtCharts::registerTypes(const char* uri) {
    qmlRegisterType<FftBarChart>(uri, 1, 0, "FftBarChart");
    qmlRegisterType<HpwBarChart>(uri, 1, 0, "HpwBarChart");
    qmlRegisterType<BarChart>(uri, 1, 0, "BarChart");
    qmlRegisterType<cBar>(uri, 1, 0, "Bar");
}


static void init(){
    QwtCharts::init();
}
Q_COREAPP_STARTUP_FUNCTION(init)
