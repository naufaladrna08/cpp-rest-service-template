#include <core/exceptions/HttpException.h>

HttpException::HttpException(int code, const std::string& message) {
  m_code = code;
  m_message = message;
  std::stringstream ss;
  ss << message;
  m_what = ss.str();
}

HttpException::HttpException(int code, const std::string& message, const std::string& what) {
  m_code = code;
  m_message = message;
  m_what = what;
}

HttpException::HttpException(int code, const std::string& message, pqxx::sql_error* sqlError) {
  m_code = code;
  m_message = message;
  std::stringstream ss;
  ss << message << " - " << sqlError->what();
  m_what = ss.str();
  m_sqlError = sqlError;
}

