#ifndef SCHEMA_H
#define SCHEMA_H
#include <string>
#include <vector>
#include <unordered_map>

class Schema {
  public:
    Schema();
    ~Schema();
    void addColumns(std::string name, std::string type);
    bool hasTimestamps();
    void hasTimestamps(bool timestamps);
    std::unordered_map<std::string, std::string> getColumns();

  private:
    std::unordered_map<std::string, std::string> m_columns;
    bool m_timestamps;
};
#endif