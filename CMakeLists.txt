cmake_minimum_required(VERSION 3.16) #версия CMake

project(NeobankServer LANGUAGES CXX) #указание, что CMake корневой для проекта

set(CMAKE_AUTOUIC ON) #создание переменной
set(CMAKE_AUTOMOC ON) #${VARIABLE} для использования переменной
set(CMAKE_AUTORCC ON) #set(VARIABLE "${VARIABLE} new text") - добавление текста к переменной

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Network Sql REQUIRED)
find_package(PostgreSQL REQUIRED)

set(SOURCES
  src/main.cpp
  src/common/constants.h
  src/test3.h
  src/pipeline/pipelinebuilder.h src/pipeline/pipelinebuilder.cpp
  src/application.h src/application.cpp

  src/network/socketwrapper.h src/network/socketwrapper.cpp
  src/network/sessionmanager.h src/network/sessionmanager.cpp
  src/network/transportlayer.h
  src/network/transportlayer.cpp

  src/context/contextwrapper.h src/context/contextwrapper.cpp
  src/context/messagecontext.h
  src/context/user.h src/context/user.cpp
  src/context/sessioncontext.h src/context/sessioncontext.cpp
  src/context/messagecontext.h src/context/messagecontext.cpp

  src/database/dbmanager.h
  src/database/dbmanager.cpp
  src/database/models/account.h
  src/database/models/transaction.h
  src/database/models/user.h
  src/database/models/authsession.h
  src/database/models/auditlogentry.h
  src/database/repositories/accountrepository.h
  src/database/repositories/accountrepository.cpp
  src/database/repositories/transactionrepository.h
  src/database/repositories/transactionrepository.cpp
  src/database/repositories/userrepository.h
  src/database/repositories/userrepository.cpp
  src/database/repositories/authsessionrepository.h
  src/database/repositories/authsessionrepository.cpp
  src/database/repositories/auditlogrepository.h
  src/database/repositories/auditlogrepository.cpp

  src/middleware/imiddleware.h
  src/middleware/router.h src/middleware/router.cpp
  src/middleware/authentification.h src/middleware/authentification.cpp
  src/middleware/authorization.h src/middleware/authorization.cpp
  src/middleware/endpointinvoker.h src/middleware/endpointinvoker.cpp
  src/middleware/logger.h src/middleware/logger.cpp
  src/middleware/session.h src/middleware/session.cpp
  src/middleware/exception.h src/middleware/exception.cpp

  src/service/authentification.h src/service/authentification.cpp
  src/service/session.h src/service/session.cpp
  src/service/iservice.h src/service/iservice.cpp
)

add_executable(NeobankServer ${SOURCES}
  src/endpoint/login.h src/endpoint/login.cpp
  src/endpoint/register.h src/endpoint/register.cpp
  src/endpoint/iendpoint.h)
#add_library
#include_directories("" "" "") - для поиска хедеров

target_link_libraries(NeobankServer PRIVATE Qt6::Core Qt::Network Qt6::Sql PostgreSQL::PostgreSQL)

include(GNUInstallDirs)
install(TARGETS NeobankServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
