#ifndef CONNECTION_H
#define CONNECTION_H
#include <iostream>
#include <string>
#include <pqxx/pqxx>

class Connection {
  public:
    static Connection& getInstance(const std::string& connectionString = "");

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    // ~Connection();

    auto execute(const std::string& query) -> pqxx::result;

  private:
    Connection();
    std::string m_connectionString;
    pqxx::connection* m_connection;
    bool m_isInitialized;
};

#endif //CONNECTION_H
