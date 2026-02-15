cmake_minimum_required(VERSION 3.16)
project(QTDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick Qml)
find_package(Boost REQUIRED COMPONENTS system unit_test_framework)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

add_executable(app
    main.cpp 
    src/currencyworker.cpp
    include/currencyworker.h    
)

target_include_directories(app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(app PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
    Qt6::Quick    
    Qt6::Qml      
    Boost::boost 
    Boost::system
    OpenSSL::SSL 
    OpenSSL::Crypto
    Threads::Threads
)

qt_add_qml_module(app
    URI MainModule
    VERSION 1.0
    QML_FILES src/main.qml
)

enable_testing()
add_subdirectory(test)


