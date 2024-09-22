#ifndef MODEL_H
#define MODEL_H
#include <core/Connection.h>
#include <core/database/Schema.h>
#include <memory>

class Model {
  public:
    Model(std::string table, std::shared_ptr<Schema> schema);
    ~Model();
    std::string getTable();
    std::shared_ptr<Schema> getSchema();
    
    void fill(std::string key, std::string value);
    int save();

    /* SELECT statements */
    template <typename T> T* find(int id);
    template <typename T> std::unique_ptr<T> findByValue(std::string field, std::string value);
    std::unique_ptr<Model> select(std::vector<std::string> columns);
    std::unique_ptr<Model> where(const std::string& column, const std::string& value);
    std::unique_ptr<Model> join(const std::string& table, const std::string& foreignKey, const std::string& primaryKey);
    std::unique_ptr<Model> orderBy(const std::string& column, const std::string& order);
    std::unique_ptr<Model> limit(int limit);

    /* SELECT executions */
    pqxx::result get();
    template <typename T> T get(std::string key);

  private:
    std::string m_table;
    std::string m_primaryKey = "id";
    std::shared_ptr<Schema> m_schema;
    bool m_hasTimestamps;
    std::unordered_map<std::string, std::string> m_preparedData;
    std::vector<std::unordered_map<std::string, std::string>> m_data;


    /* Statement Storages */
    std::vector<std::string> m_selectColumns;
    std::vector<std::string> m_whereClauses;
    std::vector<std::string> m_joins;
    std::vector<std::string> m_orderClauses;
    int m_limit = 50;

    template <typename T>
    T* findByColumnHelper(std::string column, std::string value);

    typedef struct {
      std::string table;
      std::string foreignKey;
      std::string primaryKey;
    } Relationship;

    /* 
     * Relationship Feature
     * 
     * First string is the name of the relationship and this can be 
     * anything. The second a struct that contains the table name,
     */
    std::unordered_map<std::string, Relationship> m_relation;
};

#include "Model.inl"

#endif