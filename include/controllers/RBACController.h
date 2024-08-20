#ifndef RBACCONTROLLER_H
#define RBACCONTROLLER_H
#include <core/exceptions/HttpException.h>
#include <core/StatusCodes.h>
#include <core/types.h>
#include <core/DB.h>
#include <core/Connection.h>
#include <helpers/RequestHelper.h>
#include <json.h>
#include <bcrypt.h>
#include <jwt-cpp/jwt.h>
#include <cstdlib>

class RBACController {
  public:
    /*
     * @desc: Create a new role
     * 
     * @param: string name
     * @param: string description
     */
    static void createRole(const Request& req, Response* res);

    /*
     * @desc: Create a new permission
     * 
     * @param: string name
     * @param: string description
     * @param: int roleID
     */
    static void createPermission(const Request& req, Response* res);

    /*
     * @desc: Assign a role to a user
     * 
     * @param: string userId
     * @param: string roleId
     */
    static void assignRole(const Request& req, Response* res);
};

#endif