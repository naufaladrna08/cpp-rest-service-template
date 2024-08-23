#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <core/types.h>
#include <core/DB.h>
#include <core/Connection.h>
#include <helpers/RequestHelper.h>
#include <core/exceptions/HttpException.h>
#include <json.h>
#include <bcrypt.h>
#include <jwt-cpp/jwt.h>

class ProjectController {
  public:
    /**
     * @brief [POST] Create a new project
     * 
     * Body Request:
     * @name Project Name
     * @description Project Description
     * @type Project Type (e.g. Animation, Game, Tools)
     */
    static void createProject(const Request& req, Response* res);
    // static void getProjects(const Request& req, Response* res);
    // static void getProject(const Request& req, Response* res);
    // static void updateProject(const Request& req, Response* res);
    // static void deleteProject(const Request& req, Response* res);
};

#endif