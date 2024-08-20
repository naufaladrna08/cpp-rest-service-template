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
  pqxx::nontransaction trx(*m_connection);
  pqxx::result result = trx.exec(query);
  return result;
}

auto Connection::executeWithTransaction(const std::string& query) -> pqxx::result {
  return m_transaction->exec(query);
}

void Connection::beginTransaction() {
  m_transaction = new pqxx::work(*m_connection);
}

void Connection::commitTransaction() {
  m_transaction->commit();
  delete m_transaction;
}

void Connection::rollbackTransaction() {
  m_transaction->abort();
  delete m_transaction;
}


// Connection::~Connection() {
//   if (m_connection) {
//     m_connection->close();
//     delete m_connection;
//   }
// }
