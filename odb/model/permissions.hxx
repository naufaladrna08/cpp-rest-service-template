#include <string>
#include <odb/core.hxx>
#include <boost/date_time/posix_time/posix_time.hpp>

using boost::posix_time::ptime;

#pragma db object
class permissions {
  public:
    permissions() {}
    permissions(const std::string& code, const std::string& name, unsigned long role_id, bool is_active, const long long int& created_at, const long long int& updated_at)
      : code_(code), name_(name), role_id_(role_id), is_active_(is_active), created_at_(created_at), updated_at_(updated_at) {}

    unsigned long id() const { return id_; }
    const std::string& code() const { return code_; }
    const std::string& name() const { return name_; }
    unsigned long role_id() const { return role_id_; }
    bool is_active() const { return is_active_; }
    const long long int& created_at() const { return created_at_; }
    const long long int& updated_at() const { return updated_at_; }

    void code(const std::string& code) { code_ = code; }
    void name(const std::string& name) { name_ = name; }
    void role_id(unsigned long role_id) { role_id_ = role_id; }
    void is_active(bool is_active) { is_active_ = is_active; }
    void created_at(const long long int& created_at) { created_at_ = created_at; }
    void updated_at(const long long int& updated_at) { updated_at_ = updated_at; }

  private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;

    #pragma db type("VARCHAR(255)")
    std::string code_;
    #pragma db type("VARCHAR(255)")
    std::string name_;
    unsigned long role_id_;

    bool is_active_;

    #pragma db type("TIMESTAMP")
    long long int created_at_;
    #pragma db type("TIMESTAMP")
    long long int updated_at_;
};
