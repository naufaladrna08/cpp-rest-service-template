#include <odb/core.hxx>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

class userdata {
  public:
    userdata() {}
    userdata(unsigned int user_id, std::string phone_number, std::string address, unsigned int position_id, unsigned int profile_photo_id, boost::posix_time::ptime created_at, boost::posix_time::ptime updated_at) :
      user_id_(user_id), phone_number_(phone_number), address_(address), position_id_(position_id), profile_photo_id_(profile_photo_id), created_at_(created_at), updated_at_(updated_at) {}

    unsigned int id() const { return id_; }
    unsigned int user_id() const { return user_id_; }
    std::string phone_number() const { return phone_number_; }
    std::string address() const { return address_; }
    unsigned int position_id() const { return position_id_; }
    unsigned int profile_photo_id() const { return profile_photo_id_; }
    boost::posix_time::ptime created_at() const { return created_at_; }
    boost::posix_time::ptime updated_at() const { return updated_at_; }

    void set_user_id(unsigned int user_id) { user_id_ = user_id; }
    void set_phone_number(std::string phone_number) { phone_number_ = phone_number; }
    void set_address(std::string address) { address_ = address; }
    void set_position_id(unsigned int position_id) { position_id_ = position_id; }
    void set_profile_photo_id(unsigned int profile_photo_id) { profile_photo_id_ = profile_photo_id; }
    void set_created_at(boost::posix_time::ptime created_at) { created_at_ = created_at; }
    void set_updated_at(boost::posix_time::ptime updated_at) { updated_at_ = updated_at; }

  private:
    friend class odb::access;

    #pragma db id
    unsigned int id_;
    unsigned int user_id_;
    std::string phone_number_;
    std::string address_;
    unsigned int position_id_;
    unsigned int profile_photo_id_;

    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime created_at_;
    #pragma db type("TIMESTAMP")
    boost::posix_time::ptime updated_at_;
};
