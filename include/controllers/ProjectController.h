#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <core/types.h>
#include <core/DB.h>
#include <core/Connection.h>
#include <json.h>
#include <bcrypt.h>
#include <jwt-cpp/jwt.h>

class ProjectController {
  public:
    static void createProject(const Request& req, Response* res);
    // static void getProjects(const Request& req, Response* res);
    // static void getProject(const Request& req, Response* res);
    // static void updateProject(const Request& req, Response* res);
    // static void deleteProject(const Request& req, Response* res);
};

#endif