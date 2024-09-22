#include <core/database/Model.h>

Model::Model(std::string table, std::shared_ptr<Schema> schema) {
  m_table = table;
  m_schema = schema;
}

Model::~Model() {

}

int Model::save() {
  // TODO: Before saving the data we need to make sure that the data is valid
  // according to the schema. 

  // We need to insert quote marks around the values if the column type from 
  // the schema is a string and we need to cast the value to the correct type
  // and also check for timestamps.
  bool first = true;
  std::string query = "INSERT INTO " + m_table + " (";
  std::string values = "VALUES (";

  for (const auto& [key, value] : m_preparedData) {
    if (m_schema->getColumns().find(key) == m_schema->getColumns().end()) {
      continue; // Skip if the key is not in the schema
    }

    if (!first) {
      query += ", ";
      values += ", ";
    } else {
      first = false; // After the first column, use comma before the next
    }

    query += key; // Append column name

    // Check column type and format the value accordingly
    if (m_schema->getColumns().at(key) == "string") {
      values += "'" + value + "'"; // Quote string values
    } else {
      values += value; // Assume numeric or non-quoted types
    }
  }

  // Add timestamps if required
  if (m_schema->hasTimestamps()) {
    query += ", created_at, updated_at) ";
    values += ", NOW(), NOW());";
  } else {
    query += ") ";
    values += ");";
  }

  query += values; // Complete the query

  auto result = Connection::getInstance().execute(query);
  if (result.empty()) {
    return true;
  }

  return false;
}

void Model::fill(std::string key, std::string value) {
  m_preparedData[key] = value;
}

std::string Model::getTable() {
  return m_table;
}

std::shared_ptr<Schema> Model::getSchema() {
  return m_schema;
}

std::unique_ptr<Model> Model::select(std::vector<std::string> columns) {
  m_selectColumns = columns;
  return std::make_unique<Model>(*this);
}

std::unique_ptr<Model> Model::where(const std::string& column, const std::string& value) {
  m_whereClauses.push_back(column + " = '" + value + "'");
  return std::make_unique<Model>(*this);
}

std::unique_ptr<Model> Model::join(const std::string& table, const std::string& foreignKey, const std::string& primaryKey) {
  m_joins.push_back("JOIN " + table + " ON " + m_table + "." + foreignKey + " = " + table + "." + primaryKey);
  return std::make_unique<Model>(*this);
}

std::unique_ptr<Model> Model::orderBy(const std::string& column, const std::string& order) {
  m_orderClauses.push_back("ORDER BY " + column + " " + order);
  return std::make_unique<Model>(*this);
}

std::unique_ptr<Model> Model::limit(int limit) {
  m_limit = limit;
  return std::make_unique<Model>(*this);
}

pqxx::result Model::get() {
  std::string query = "SELECT ";
  if (m_selectColumns.empty()) {
    query += "* ";
  } else {
    bool first = true;
    for (const auto& column : m_selectColumns) {
      if (!first) {
        query += ", ";
      } else {
        first = false;
      }

      query += column;
    }
  }

  query += " FROM " + m_table;

  if (!m_joins.empty()) {
    for (const auto& join : m_joins) {
      query += " " + join;
    }
  }

  if (!m_whereClauses.empty()) {
    query += " WHERE ";
    bool first = true;
    for (const auto& where : m_whereClauses) {
      if (!first) {
        query += " AND ";
      } else {
        first = false;
      }

      query += where;
    }
  }

  if (!m_orderClauses.empty()) {
    for (const auto& order : m_orderClauses) {
      query += " " + order;
    }
  }

  query += " LIMIT " + std::to_string(m_limit) + ";";

  std::cout <<  query << std::endl;

  auto result = Connection::getInstance().execute(query);
  return result;
}