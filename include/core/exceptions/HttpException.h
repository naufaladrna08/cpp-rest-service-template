#ifndef HTTP_EXCEPTION_H
#define HTTP_EXCEPTION_H
#include <exception>
#include <string>
#include <sstream>
#include <pqxx/pqxx>

class HttpException : public std::exception {
  public:
    HttpException(int code, const std::string& message);
    HttpException(int code, const std::string& message, const std::string& what);
    HttpException(int code, const std::string& message, pqxx::sql_error* sqlError);
    
    [[nodiscard]] int code() const { return m_code; }
    [[nodiscard]] const std::string& message() const { return m_message; }
    [[nodiscard]] const char* what() const noexcept override { return m_what.c_str(); }

  private:
    int m_code;
    std::string m_message;
    std::string m_what;
    pqxx::sql_error* m_sqlError;
};

#endif