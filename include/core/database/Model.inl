#include "Model.h"

template <typename T>
T Model::get(std::string key) {
  return m_data[0][key];
}

template <typename T>
T* Model::find(int id) {
  return Model::findByColumnHelper<T>("id", std::to_string(id));
}

template <typename T>
std::unique_ptr<T> Model::findByValue(std::string field, std::string value) {
  return std::unique_ptr<T>(Model::findByColumnHelper<T>(field, value));
}

template <typename T>
T* Model::findByColumnHelper(std::string column, std::string value) {
  std::string query = "SELECT * FROM " + m_table + " WHERE " + column + " = '" + value + "'";
  auto result = Connection::getInstance().execute(query);
  if (result.empty()) {
    return {};
  }

  auto row = result.at(0);
  std::unordered_map<std::string, std::string> data;
  T* t = new T();
  for (auto const &field: row) {
    t->fill(field.name(), field.c_str());
  }

  return t;
}