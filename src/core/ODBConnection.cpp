#include <core/ODBConnection.h>

std::shared_ptr<odb::pgsql::database> OdbConnection::getInstance(const std::string& connectionString) {
  static OdbConnection instance;

  std::cout << "Getting instance... " << connectionString << std::endl;

  if (!instance.m_initialized) {
    instance.m_instance = std::make_shared<odb::pgsql::database>(connectionString);

    std::cout << "Connected to database" << std::endl;

    instance.m_initialized = true;
  }

  return instance.m_instance;
}
