#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QQmlApplicationEngine> 
#include <QQmlContext>  

#include "currencyworker.h"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;

    CurrencyWorker worker;    
   
    engine.rootContext()->setContextProperty("worker", &worker);

    const QUrl url(QStringLiteral("qrc:/MainModule/src/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}