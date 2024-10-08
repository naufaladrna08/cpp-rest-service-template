#include <odb/core.hxx>
#include <ctime>
#include <boost/date_time/posix_time/posix_time.hpp>

class permission_role {
  private:
    friend class odb::access;
    #pragma db id auto
    unsigned int id_;

    unsigned int permission_id_;
    unsigned int role_id_;
    bool is_active_;

    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime created_at_;
    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime updated_at_;
};
