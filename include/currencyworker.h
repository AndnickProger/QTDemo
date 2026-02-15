#ifndef CURRENCYWORKER_H
#define CURRENCYWORKER_H

#include <QObject>
#include <QString>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>


class CurrencyWorker : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

public:
    explicit CurrencyWorker(QObject *parent = nullptr) : QObject(parent), m_loading(false) {}

    QString displayText() const { return m_text; }
    bool isLoading() const { return m_loading; }

    Q_INVOKABLE void fetch() {
        if (m_loading) return;
        m_loading = true;
        emit isLoadingChanged();
        run(); 
    }

signals:
    void displayTextChanged();
    void isLoadingChanged();

public slots:
    void onDataReady(QString text) {
        m_text = text;
        m_loading = false;
        emit displayTextChanged();
        emit isLoadingChanged();
    }

private:
    void run(); 
    boost::asio::awaitable<void> fetch_task();
    
    QString m_text;
    bool m_loading;
};

#endif