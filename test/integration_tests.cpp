#define BOOST_TEST_MODULE CurrencyIntegrationTests
#include <boost/test/included/unit_test.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "currencyworker.h"

BOOST_AUTO_TEST_CASE(test_qml_binding_integration) {
    int argc = 1;
    char* argv[] = {(char*)"test"};
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    CurrencyWorker worker;
    engine.rootContext()->setContextProperty("worker", &worker);
   
    QVariant var = engine.rootContext()->contextProperty("worker");
    BOOST_REQUIRE(var.isValid());
    
    QObject* obj = qvariant_cast<QObject*>(var);
    BOOST_REQUIRE(obj != nullptr);

    bool isLoading = obj->property("isLoading").toBool();
    BOOST_CHECK_EQUAL(isLoading, false);
    
    worker.fetch(); 
    BOOST_CHECK_EQUAL(obj->property("isLoading").toBool(), true);
}
