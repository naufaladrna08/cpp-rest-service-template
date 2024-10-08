#include <odb/core.hxx>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

class positions {
  public:
    positions() {}
    positions(const std::string& code, const std::string& name)
      : code_(code), name_(name) {}

    unsigned long id() const { return id_; }

    const std::string& code() const { return code_; }
    void code(const std::string& code) { code_ = code; }

    const std::string& name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    bool is_active() const { return is_active_; }
    void is_active(bool is_active) { is_active_ = is_active; }

    const boost::posix_time::ptime& created_at() const { return created_at_; }
    void created_at(const boost::posix_time::ptime& created_at) { created_at_ = created_at; }

    const boost::posix_time::ptime& updated_at() const { return updated_at_; }
    void updated_at(const boost::posix_time::ptime& updated_at) { updated_at_ = updated_at; }

  private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;
    std::string code_;
    std::string name_;
    bool is_active_;

    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime created_at_;
    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime updated_at_;
};
