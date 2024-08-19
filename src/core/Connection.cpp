#include <core/Connection.h>

Connection& Connection::getInstance(const std::string& connectionString) {
  static Connection instance;

  // Only set the connection string during the first call
  if (!instance.m_isInitialized) {
    if (connectionString.empty()) {
      throw std::runtime_error("Connection string must be provided for the first initialization.");
    }
    instance.m_connectionString = connectionString;
    instance.m_connection = new pqxx::connection(connectionString);
    if (instance.m_connection->is_open()) {
      std::cout << "Opened database successfully: " << instance.m_connection->dbname() << std::endl;
    } else {
      throw std::runtime_error("Failed to open database");
    }

    instance.m_isInitialized = true;
  }

  return instance;
}

Connection::Connection() : m_isInitialized(false) {}

auto Connection::execute(const std::string& query) -> pqxx::result {
  pqxx::work txn(*m_connection);
  pqxx::result result = txn.exec(query);
  txn.commit();

  return result;
}

// Connection::~Connection() {
//   if (m_connection) {
//     m_connection->close();
//     delete m_connection;
//   }
// }
