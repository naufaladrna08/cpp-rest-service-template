#include <odb/core.hxx>
#include <string>

#pragma db object
class roles {
  public:
    roles(const std::string& code, const std::string& name, const std::string& type, unsigned int project_id, bool is_active, const long long int& created_at, const long long int& updated_at) : code_(code), name_(name), type_(type), project_id_(project_id), is_active_(is_active), created_at_(created_at), updated_at_(updated_at) {}

    unsigned int id() const { return id_; }
    const std::string& code() const { return code_; }
    void code(const std::string& code) { code_ = code; }
    const std::string& name() const { return name_; }
    void name(const std::string& name) { name_ = name; }
    const std::string& type() const { return type_; }
    void type(const std::string& type) { type_ = type; }
    unsigned int project_id() const { return project_id_; }
    void project_id(unsigned int project_id) { project_id_ = project_id; }
    bool is_active() const { return is_active_; }
    void is_active(bool is_active) { is_active_ = is_active; }
    const long long int& created_at() const { return created_at_; }
    void created_at(const long long int& created_at) { created_at_ = created_at; }
    const long long int& updated_at() const { return updated_at_; }
    void updated_at(const long long int& updated_at) { updated_at_ = updated_at; }

  private:
    friend class odb::access;
    roles() {}
    #pragma db id auto
    unsigned int id_;
    std::string code_;
    std::string name_;
    std::string type_; // enum
    unsigned int project_id_;

    bool is_active_;

    #pragma db type("TIMESTAMP")
    long long int created_at_;
    #pragma db type("TIMESTAMP")
    long long int updated_at_;
};
