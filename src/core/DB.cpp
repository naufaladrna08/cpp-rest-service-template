#include <core/DB.h>

DB::DB() {}

DB::~DB() {}

DB* DB::prepare(const std::string& query) {
  DB* db = new DB();
  db->m_query = query;
  return db;
}

DB* DB::bind(const std::string& key, const std::string& value) {
  std::string::size_type pos = m_query.find(key);
  if (pos != std::string::npos) {
    m_query.replace(pos, key.length(), value);
  }
  return this;
}

DB* DB::bind(int value) {
  std::string::size_type pos = m_query.find("?");
  if (pos != std::string::npos) {
    m_query.replace(pos, 1, std::to_string(value));
  }
  return this;
}

std::string DB::get() {
  return m_query;
}