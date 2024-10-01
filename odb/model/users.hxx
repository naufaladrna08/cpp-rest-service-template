#include <string>
#include <odb/core.hxx>

#pragma db object
class users {
  public:
    users(std::string name, std::string username, std::string password) {
      this->name_ = name;
      this->username_ = username;
      this->password_ = password;
    }

    const std::string& get_name() const;
    void set_name(const std::string& name);

    const std::string& get_username() const;
    void set_username(const std::string& username);

    const std::string& get_password() const;
    void set_password(const std::string& password);

  private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;
    std::string name_;
    std::string username_;
    std::string password_;
};