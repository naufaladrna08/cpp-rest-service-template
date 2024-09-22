#include <models/User.h>

User::User() : Model("users", std::make_shared<Schema>()) {
  getSchema()->addColumns("id", "integer");
  getSchema()->addColumns("name", "string");
  getSchema()->addColumns("username", "string");
  getSchema()->addColumns("password", "string");
  getSchema()->hasTimestamps(true);
}

User::~User() {

}