#ifndef DB_H
#define DB_H
#include <vector>
#include <string>

// SQL Builder class
// Usage Example;
// std::string query = DB::prepare("SELECT * FROM users WHERE id = ?")->bind(1)->get();
// std::string query = DB::prepare("SELECT * FROM users WHERE id = :id")->bind(":id", 1)->get();
// std::string query = DB::prepare("INSERT INTO users (name, email) VALUES (?, ?)")->bind("John Doe", "johndoe@mail.com")->get();
// std::string query = DB::prepare("UPDATE users SET name = ? WHERE id = ?")->bind("John Doe", 1)->get();

class DB {
  public:
    DB();
    ~DB();

    static DB* prepare(const std::string& query);
    DB* bind(const std::string& key, const std::string& value);
    DB* bind(int value);
    std::string get();

  private:
    std::string m_query;
    std::vector<std::string> m_bindings;
};

#endif //DB_H
