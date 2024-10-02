cmake_minimum_required(VERSION 3.2)
project(template_service)

set(CMAKE_CXX_STANDARD 17)

# Libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

# Boost
find_package(Boost REQUIRED COMPONENTS system)

# Third-party libraries
add_subdirectory(vendor/dotenv)
add_subdirectory(vendor/bcrypt)
add_subdirectory(vendor/json-c)
add_subdirectory(vendor/jwt-cpp)

include_directories(
  vendor/jwt-cpp/include
  vendor/dotenv/include
  vendor/bcrypt/include
  vendor/json-c/include
  odb/src
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${LIBPQXX_INCLUDE_DIRS}
)

# ODB Sources
set(ODB_SOURCES
  odb/src/users-odb.cxx
)

set(APP_CORE_SOURCES
  src/core/exceptions/HttpException.cpp
  src/core/exceptions/HttpException.cpp
  src/core/Connection.cpp
  src/core/DB.cpp
)

add_executable(template_service 
  main.cpp  
  src/middleware/JwtAuth.cpp
  src/controllers/UserController.cpp
  src/helpers/RequestHelper.cpp
  ${APP_CORE_SOURCES}
  ${ODB_SOURCES}
)

target_link_libraries(template_service ${LIBPQXX_LIBRARIES} ${Boost_LIBRARIES} pthread cpp_dotenv bcrypt json-c jwt-cpp::jwt-cpp odb odb-pgsql)