#include "currencyworker.h"

#include <iconv.h>
#include <thread>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;


std::string win1251_to_utf8_impl(const std::string& input) {
    iconv_t cd = iconv_open("UTF-8", "CP1251");
    if (cd == (iconv_t)-1) return input;
    
    char* in_ptr = const_cast<char*>(input.data());
    size_t in_bytes = input.size();
    
    std::string output(input.size() * 2, '\0');
    char* out_ptr = &output[0];
    size_t out_bytes = output.size();
    
    iconv(cd, &in_ptr, &in_bytes, &out_ptr, &out_bytes);
    output.resize(output.size() - out_bytes);
    iconv_close(cd);
    return output;
}

void CurrencyWorker::run() {
    
    std::thread([this]() {
        try {
            net::io_context ioc;
            net::co_spawn(ioc, fetch_task(), net::detached);
            ioc.run();
        } catch (std::exception& e) {
            
            QMetaObject::invokeMethod(this, "onDataReady", 
                                     Qt::QueuedConnection, 
                                     Q_ARG(QString, QString("Ошибка: ") + e.what()));
        }
    }).detach();
}

net::awaitable<void> CurrencyWorker::fetch_task() {
    auto executor = co_await net::this_coro::executor;
    ssl::context ctx{ssl::context::tlsv12_client};
    ctx.set_default_verify_paths();
    
   
    beast::ssl_stream<beast::tcp_stream> stream(executor, ctx);
    tcp::resolver resolver(executor);

    auto const results = co_await resolver.async_resolve("www.cbr.ru", "443", net::use_awaitable);
    co_await beast::get_lowest_layer(stream).async_connect(results, net::use_awaitable);
    
    SSL_set_tlsext_host_name(stream.native_handle(), "www.cbr.ru");
    co_await stream.async_handshake(ssl::stream_base::client, net::use_awaitable);
    
    http::request<http::string_body> req{http::verb::get, "/scripts/XML_daily.asp", 11};
    req.set(http::field::host, "www.cbr.ru");
    req.set(http::field::user_agent, "BoostBeastClient");

    co_await http::async_write(stream, req, net::use_awaitable);

    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    co_await http::async_read(stream, buffer, res, net::use_awaitable);
    
    std::stringstream ss(res.body());
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(ss, pt);

    QString report = "Данные ЦБ РФ:\n\n";
    for (auto const& [name, tree] : pt.get_child("ValCurs")) {
        if (name == "Valute") {
            std::string nominal = tree.get<std::string>("Nominal");
            std::string raw_name = tree.get<std::string>("Name");
            std::string value = tree.get<std::string>("Value");
            
            std::string utf8_name = win1251_to_utf8_impl(raw_name);
            
            report += QString("%1 %2 = %3 руб.\n")
                        .arg(nominal.c_str())
                        .arg(utf8_name.c_str())
                        .arg(value.c_str());
        }
    }
  
    QMetaObject::invokeMethod(this, "onDataReady", 
                             Qt::QueuedConnection, 
                             Q_ARG(QString, report));

    beast::error_code ec;
    stream.shutdown(ec);
    co_return;
}