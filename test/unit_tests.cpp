#define BOOST_TEST_MODULE UnitTests
#include <boost/test/included/unit_test.hpp>

#include "currencyworker.h"
#include <QSignalSpy>
#include <QGuiApplication>

struct QtSetup {
    QtSetup() { 
        int argc = 1;
        char* argv[] = {(char*)"test"};
        app = new QCoreApplication(argc, argv); 
    }
    ~QtSetup() { delete app; }
    QCoreApplication* app;
};

BOOST_GLOBAL_FIXTURE(QtSetup);

BOOST_AUTO_TEST_CASE(test_initial_state) {
    CurrencyWorker worker;
    BOOST_CHECK_EQUAL(worker.isLoading(), false);
    BOOST_CHECK_EQUAL(worker.displayText().toStdString(), "");
}

BOOST_AUTO_TEST_CASE(test_data_ready_signal) {
    CurrencyWorker worker;
    QSignalSpy spy(&worker, &CurrencyWorker::displayTextChanged);
    QSignalSpy loadingSpy(&worker, &CurrencyWorker::isLoadingChanged);
  
    worker.onDataReady("Test Data");

    BOOST_CHECK_EQUAL(spy.count(), 1);
    BOOST_CHECK_EQUAL(worker.displayText().toStdString(), "Test Data");
    BOOST_CHECK_EQUAL(worker.isLoading(), false);
}