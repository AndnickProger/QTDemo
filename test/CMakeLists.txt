set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Test)

add_executable(unit_tests unit_tests.cpp ../src/currencyworker.cpp ../include/currencyworker.h)
target_include_directories(unit_tests PRIVATE ../include)
#target_link_libraries(unit_tests PRIVATE Qt6::Core Qt6::Test Boost::unit_test_framework OpenSSL::SSL Threads::Threads)
target_link_libraries(unit_tests PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Test 
    Boost::unit_test_framework 
    OpenSSL::SSL 
    Threads::Threads
)

add_executable(integration_tests integration_tests.cpp ../src/currencyworker.cpp ../include/currencyworker.h)
target_include_directories(integration_tests PRIVATE ../include)
target_link_libraries(integration_tests PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Boost::unit_test_framework OpenSSL::SSL Threads::Threads)

add_test(NAME UnitTests COMMAND unit_tests)
add_test(NAME IntegrationTests COMMAND integration_tests)
