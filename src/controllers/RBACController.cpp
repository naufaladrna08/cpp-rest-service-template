#include <controllers/RBACController.h>

void RBACController::createRole(const Request& req, Response* res) {
  // Create a new role
  json_object* body = json_tokener_parse(req.body().c_str());
  std::string name = RequestHelper::getJsonSafeString(body, "name");
  std::string description = RequestHelper::getJsonSafeString(body, "description");

  std::string query = DB::prepare("INSERT INTO roles (name, description, created_at) VALUES (':name', ':description', NOW())")
    ->bind(":name", name)
    ->bind(":description", description)
    ->get();

  json_object* response = json_object_new_object();

  try {
    std::string queryCheck = DB::prepare("SELECT * FROM roles WHERE name = ':name'")
      ->bind(":name", name)
      ->get();

    auto result = Connection::getInstance().execute(queryCheck);
    if (!result.empty()) {
      res->result(boost::beast::http::status::conflict);
      json_object_object_add(response, "error", json_object_new_string("Role already exists."));
      res->body() = json_object_to_json_string(response);
      res->prepare_payload();
      return;
    }

    auto createResult = Connection::getInstance().execute(query);
    if (createResult.empty()) {
      res->result(boost::beast::http::status::ok);
      json_object_object_add(response, "message", json_object_new_string("Role created successfully."));
    }

  } catch (std::exception e) {
    res->result(boost::beast::http::status::internal_server_error);
    json_object_object_add(response, "error", json_object_new_string("An error occurred while creating the role."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  res->result(boost::beast::http::status::ok);
  json_object_object_add(response, "message", json_object_new_string("Role created successfully."));
  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}

void RBACController::createPermission(const Request& req, Response* res) {
  // Create a new permission
  json_object* body = json_tokener_parse(req.body().c_str());
  json_object* response = json_object_new_object();
  std::string name = RequestHelper::getJsonSafeString(body, "name");
  std::string description = RequestHelper::getJsonSafeString(body, "description");
  int roleID = RequestHelper::getJsonSafeInt(body, "roleID");

  /* Begin Transaction */
  Connection::getInstance().beginTransaction();

  try {
    std::string queryCheck = DB::prepare("SELECT * FROM role_permissions WHERE role_id = :roleID")
      ->bind(":roleID", std::to_string(roleID))
      ->get();

    auto result = Connection::getInstance().executeWithTransaction(queryCheck);
    if (!result.empty()) {
      throw HttpException(HTTP_BAD_REQUEST, "Permission already exists.");
    }

    for (const auto& row : result) {
      int id = row["id"].as<int>();
      std::string queryCheckPermission = DB::prepare("SELECT * FROM permissions WHERE id = ':id'")
        ->bind(":id", std::to_string(id))
        ->get();
    
      auto resultPermission = Connection::getInstance().executeWithTransaction(queryCheckPermission);
      if (!resultPermission.empty()) {
        throw HttpException(HTTP_BAD_REQUEST, "Permission already exists.");
      }
    }

    std::string query = DB::prepare("INSERT INTO permissions (id, name, description, created_at) VALUES (DEFAULT, ':name', ':description', NOW()) RETURNING id;")
      ->bind(":name", name)
      ->bind(":description", description)
      ->get();

    auto createResult = Connection::getInstance().executeWithTransaction(query);
    if (!createResult.empty()) {
      std::string queryInsertRolePermission = DB::prepare("INSERT INTO role_permissions (role_id, permission_id) VALUES (:roleID, :permissionID) RETURNING id;")
        ->bind(":roleID", std::to_string(roleID))
        ->bind(":permissionID", std::to_string(createResult[0]["id"].as<int>()))
        ->get();

      auto insertResult = Connection::getInstance().executeWithTransaction(queryInsertRolePermission);
      if (!insertResult.empty()) {
        Connection::getInstance().commitTransaction();
        res->result(boost::beast::http::status::ok);
        json_object_object_add(response, "message", json_object_new_string("Permission created successfully."));
      } else {
        throw HttpException(HTTP_INTERNAL_SERVER_ERROR, "Failed to create permission.");
      }
    } else {
      throw HttpException(HTTP_INTERNAL_SERVER_ERROR, "Failed to create permission.");
    }
  } catch (const HttpException& e) {
    Connection::getInstance().rollbackTransaction();
    res->result(boost::beast::http::int_to_status(e.code()));
    json_object_object_add(response, "code", json_object_new_int(e.code()));
    json_object_object_add(response, "error", json_object_new_string("An error occurred while creating the permission."));
    json_object_object_add(response, "message", json_object_new_string(e.what()));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  res->result(boost::beast::http::status::ok);
  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}

void RBACController::assignRole(const Request& req, Response* res) {
  json_object* body = json_tokener_parse(req.body().c_str());
  json_object* response = json_object_new_object();
  int userID = RequestHelper::getJsonSafeInt(body, "userID");
  int roleID = RequestHelper::getJsonSafeInt(body, "roleID");

  try {
    std::string queryCheck = DB::prepare("SELECT * FROM user_roles WHERE user_id = :userID AND role_id = :roleID")
      ->bind(":userID", std::to_string(userID))
      ->bind(":roleID", std::to_string(roleID))
      ->get();

    auto result = Connection::getInstance().execute(queryCheck);
    if (!result.empty()) {
      throw HttpException(HTTP_BAD_REQUEST, "Role already assigned to user.");
    }

    std::string query = DB::prepare("INSERT INTO user_roles (user_id, role_id) VALUES (:userID, :roleID) RETURNING id;")
      ->bind(":userID", std::to_string(userID))
      ->bind(":roleID", std::to_string(roleID))
      ->get();

    auto createResult = Connection::getInstance().execute(query);
    if (createResult.empty()) {
      throw HttpException(HTTP_INTERNAL_SERVER_ERROR, "Failed to assign role.");
    }
  } catch (const HttpException& e) {
    res->result(boost::beast::http::int_to_status(e.code()));
    json_object_object_add(response, "code", json_object_new_int(e.code()));
    json_object_object_add(response, "error", json_object_new_string("An error occurred while assigning the role."));
    json_object_object_add(response, "message", json_object_new_string(e.what()));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  res->result(boost::beast::http::status::ok);
  json_object_object_add(response, "message", json_object_new_string("Role assigned successfully."));
  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}