cmake_minimum_required(VERSION 3.16) #версия CMake

project(NeobankServer LANGUAGES CXX) #указание, что CMake корневой для проекта

set(CMAKE_AUTOUIC ON) #создание переменной
set(CMAKE_AUTOMOC ON) #${VARIABLE} для использования переменной
set(CMAKE_AUTORCC ON) #set(VARIABLE "${VARIABLE} new text") - добавление текста к переменной

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Network Sql REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(PostgreSQL REQUIRED)

set(SOURCES
  src/main.cpp
  src/network/protocols/bankingprotocol.h src/network/protocols/bankingprotocol.cpp
  src/network/protocols/messageparser.h src/network/protocols/messageparser.cpp
  src/core/applicationserver.h src/core/applicationserver.cpp
  src/core/servermanager.h src/core/servermanager.cpp
  src/network/sessions/sessionmanager.h src/network/sessions/sessionmanager.cpp
  src/network/sessions/clientsession.h src/network/sessions/clientsession.cpp
  src/network/ssl/sslmanager.h src/network/ssl/sslmanager.cpp
  src/network/ssl/certificatemanager.h src/network/ssl/certificatemanager.cpp
  src/security/encryption/aesencryptor.h src/security/encryption/aesencryptor.cpp
  src/security/encryption/hashing.h src/security/encryption/hashing.cpp
  src/security/authentification/authmanager.h src/security/authentification/authmanager.cpp
  src/security/authentification/tokenmanager.h src/security/authentification/tokenmanager.cpp
  src/security/audit/auditor.h src/security/audit/auditor.cpp
  src/security/audit/auditlogger.h src/security/audit/auditlogger.cpp
  src/business/services/accountservice.h src/business/services/accountservice.cpp
  src/business/services/transactionservice.h src/business/services/transactionservice.cpp
  src/business/services/reportservice.h src/business/services/reportservice.cpp
  src/business/processors/transactionprocessor.h src/business/processors/transactionprocessor.cpp
  src/business/processors/paymentprocessor.h src/business/processors/paymentprocessor.cpp
  src/common/types.h
  src/common/constants.h
  src/common/utils.h src/common/utils.cpp
  src/database/dbconnection.h src/database/dbconnection.cpp
  src/database/dbmanager.h src/database/dbmanager.cpp
)

add_executable(NeobankServer
  ${SOURCES}
  src/database/models/account.h src/database/models/account.cpp
  src/database/models/transaction.h src/database/models/transaction.cpp
  src/database/models/user.h src/database/models/user.cpp
  src/database/repositories/accountrepository.h src/database/repositories/accountrepository.cpp
  src/database/repositories/transactionrepository.h src/database/repositories/transactionrepository.cpp


)
#add_library
#include_directories("" "" "") - для поиска хедеров

target_link_libraries(NeobankServer PRIVATE Qt6::Core Qt::Network Qt6::Sql PostgreSQL::PostgreSQL)
target_link_libraries(NeobankServer PRIVATE Qt6::Core)
target_link_libraries(NeobankServer PRIVATE Qt6::Core)
target_link_libraries(NeobankServer PRIVATE Qt6::Core)
target_link_libraries(NeobankServer PRIVATE Qt6::Core)
target_link_libraries(NeobankServer PRIVATE Qt6::Core)
target_link_libraries(NeobankServer PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS NeobankServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
