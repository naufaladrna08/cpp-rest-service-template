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
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${LIBPQXX_INCLUDE_DIRS}
)

add_executable(template_service main.cpp
  include/controllers/UserController.h
  include/controllers/RBACController.h
  include/controllers/ProjectController.h
  include/core/types.h
  include/core/DB.h
  include/core/Connection.h
  include/core/exceptions/HttpException.h
  include/core/database/Schema.h
  include/core/database/Model.h
  include/helpers/RequestHelper.h
  include/middleware/JwtAuth.h
  src/core/Connection.cpp
  src/core/exceptions/HttpException.cpp
  src/core/database/Schema.cpp
  src/core/database/Model.cpp
  src/helpers/RequestHelper.cpp

  src/models/User.cpp
  src/controllers/UserController.cpp
  src/controllers/RBACController.cpp
  src/controllers/ProjectController.cpp
  src/middleware/JwtAuth.cpp
  src/core/DB.cpp
)

target_link_libraries(template_service ${LIBPQXX_LIBRARIES} ${Boost_LIBRARIES} pthread cpp_dotenv bcrypt json-c jwt-cpp::jwt-cpp)