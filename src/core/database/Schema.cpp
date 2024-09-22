#include <core/database/Schema.h>

Schema::Schema() {}
Schema::~Schema() {}

bool Schema::hasTimestamps() {
  return m_timestamps;
}

void Schema::hasTimestamps(bool timestamps) {
  m_timestamps = timestamps;
}

void Schema::addColumns(std::string name, std::string type) {
  m_columns[name] = type;
}

std::unordered_map<std::string, std::string> Schema::getColumns() {
  return m_columns;
}