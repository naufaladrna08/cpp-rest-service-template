#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H
#include <core/types.h>
#include <core/DB.h>
#include <core/Connection.h>
#include <core/exceptions/HttpException.h>
#include <helpers/RequestHelper.h>
#include <json.h>
#include <bcrypt.h>
#include <jwt-cpp/jwt.h>
#include <models/User.h>

class UserController {
  public:
    static void registerUser(Request req, Response* res);
    static void login(Request req, Response* res);
    static void getAllUsers(Request req, Response* res);
    static void profile(Request req, Response* res);
};

#endif //USERCONTROLLER_H
