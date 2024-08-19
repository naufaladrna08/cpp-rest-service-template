#ifndef JWTAUTH_H
#define JWTAUTH_H
#include <jwt-cpp/jwt.h>
#include <json.h>
#include <core/types.h>
#include <core/DB.h>
#include <core/Connection.h>

class JwtAuth {
  public:
    static bool verifyToken(const Request& req, Response* res);
};

#endif