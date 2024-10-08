#ifndef ODB_CONNECTION_H
#define ODB_CONNECTION_H

#include <odb/pgsql/database.hxx>  // ODB PostgreSQL Database
#include <memory>                  // For std::shared_ptr
#include <iostream>
#include <dotenv.h>

class OdbConnection {
  public:
    static std::shared_ptr<odb::pgsql::database> getInstance(const std::string& connectionString = "");

  private:
    std::shared_ptr<odb::pgsql::database> m_instance;

    // Flag for thread-safe initialization
    bool m_initialized = false;
};

#endif
