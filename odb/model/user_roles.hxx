#include <odb/core.hxx>
#include <boost/date_time/posix_time/posix_time.hpp>

#pragma db object
class user_roles {
  public:
    user_roles();
    user_roles(unsigned int user_id, unsigned int role_id, bool is_active)
      : user_id_(user_id), role_id_(role_id), is_active_(is_active) {}

    unsigned long id() const { return id_; }
    unsigned int user_id() const { return user_id_; }
    unsigned int role_id() const { return role_id_; }
    bool is_active() const { return is_active_; }
    boost::posix_time::ptime created_at() const { return created_at_; }
    boost::posix_time::ptime updated_at() const { return updated_at_; }

    void set_user_id(unsigned int user_id) { user_id_ = user_id; }
    void set_role_id(unsigned int role_id) { role_id_ = role_id; }
    void set_is_active(bool is_active) { is_active_ = is_active; }
    void set_created_at(boost::posix_time::ptime created_at) { created_at_ = created_at; }
    void set_updated_at(boost::posix_time::ptime updated_at) { updated_at_ = updated_at; }

  private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;
    unsigned int user_id_;
    unsigned int role_id_;
    bool is_active_;

    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime created_at_;
    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime updated_at_;
};
