#ifndef JWTAUTH_H
#define JWTAUTH_H
#include <jwt-cpp/jwt.h>
#include <json.h>
#include <core/types.h>
#include <core/DB.h>
#include <core/Connection.h>
#include <middleware/JwtAuth.h>
#include <string>

typedef struct {
  std::string role;
  std::string permission; // If specified
} MiddlewareUserdata;

class JwtAuth {
  public:
    static bool verifyToken(const Request& req, Response* res, void* userdata);
    static bool verifyTokenWithRole(const Request& req, Response* res, void* userdata);
};

#endif