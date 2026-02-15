# QTDemo
Short description

A cross‑platform desktop application that displays current exchange rates according to the Central Bank of the Russian Federation. Implemented in Qt (QML + C++) with asynchronous data loading via Boost.Beast.
Functionality

    Getting XML data from the official API of the Central Bank of the Russian Federation (cbr.ru/scripts/XML_daily.asp ).

    Asynchronous upload via HTTPS (without blocking the interface).

    Transcoding data from Windows‑1251 to UTF‑8 for correct display of Cyrillic alphabet.

    Displaying a list of currencies: face value, name, exchange rate in rubles.

    Indication of the loading status (lock button, spinner).

    A clean, responsive QML interface.

Technologies

    Qt 5/6 (QML, Qt Quick Controls, QObject, Signals/slots).

    Boost.Beast is an HTTP/HTTPS client based on Boost.Asio.

    Boost.PropertyTree — parsing XML.

    iconv — transcoding of text data.

    C++17 — Application logic.

    QML is a declarative interface.

Project structure

    currencyworker.h / currencyworker.cpp

        The CurrencyWorker class (the successor of QObject).

        Asynchronous data loading via Boost.Beast.

        DisplayText and isLoading properties for binding to QML.

        The fetch() method for starting the download.

    main.cpp

        The entry point.

        Initialization of QGuiApplication and QQmlApplicationEngine.

        Passing a CurrencyWorker instance to a QML context.

        QML loading error handling.

    main.qml

        The interface is based on Qt Quick:

            The "Get exchange rates" button (is blocked during the download).

            The scroll area with the text of the courses.

            The loading spinner.

        Binding to worker from C++.

    Resources (qrc)

        Inclusion of main.qml in the executable file.

How to launch

    Install the dependencies:

        Qt 5.15+ (or Qt 6).

        Boost 1.70+ (with collected beast, asio, property_tree).

        libiconv (for transcoding).
