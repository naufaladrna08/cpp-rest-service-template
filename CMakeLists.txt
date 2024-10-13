cmake_minimum_required(VERSION 3.28)
project(template_service)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-deprecated-declarations")

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
add_subdirectory(vendor/ws)

include_directories(
  vendor/jwt-cpp/include
  vendor/dotenv/include
  vendor/bcrypt/include
  vendor/json-c/include
  vendor/ws/include
  odb/src
  odb/model
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${LIBPQXX_INCLUDE_DIRS}
)

# ODB Sources
set(ODB_SOURCES
  odb/src/users-odb.cxx
  odb/src/userdatas-odb.cxx
  odb/src/permissions-odb.cxx
  odb/src/permission_roles-odb.cxx
  odb/src/positions-odb.cxx
  odb/src/roles-odb.cxx
)

add_executable(template_service src/main.cpp
  ${ODB_SOURCES}
)

target_link_libraries(template_service 
  ws pq pthread cpp_dotenv bcrypt json-c jwt-cpp::jwt-cpp odb odb-pgsql 
  ${LIBPQXX_LIBRARIES} 
  ${Boost_LIBRARIES}
)
