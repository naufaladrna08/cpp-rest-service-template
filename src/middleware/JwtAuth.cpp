#include <middleware/JwtAuth.h>

bool JwtAuth::verifyToken(const Request& req, Response* res, void* userdata) {
  if (userdata != nullptr) {
    MiddlewareUserdata* data = (MiddlewareUserdata*) userdata;
    std::cout << data->role << std::endl;
  }

  std::string token = req.base().at("Authorization");
  if (token.empty()) {
    res->result(boost::beast::http::status::unauthorized);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("No token provided."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return false;
  }

  std::string secret = "secret";
  
  try {
    auto decoded = jwt::decode(token);
    auto payload = decoded.get_payload_claim("username");
    std::string username = payload.as_string();

    std::string query = DB::prepare("SELECT name, username FROM users WHERE username = ':username'")
      ->bind(":username", username)
      ->get();

    auto result = Connection::getInstance().execute(query);
    if (result.empty()) {
      res->result(boost::beast::http::status::not_found);
      json_object* response = json_object_new_object();
      json_object_object_add(response, "error", json_object_new_string("User not found."));
      res->body() = json_object_to_json_string(response);
      res->prepare_payload();
      return false;
    }

  } catch (const std::exception& e) {
    res->result(boost::beast::http::status::unauthorized);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Invalid token."));
    json_object_object_add(response, "message", json_object_new_string(e.what()));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return false;
  }

  return true;
}

bool JwtAuth::verifyTokenWithRole(const Request& req, Response* res, void* userdata) {
  if (userdata == nullptr) {
    res->result(boost::beast::http::status::forbidden);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Access denied."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return false;
  }

  std::string token = req.base().at("Authorization");
  if (token.empty()) {
    res->result(boost::beast::http::status::unauthorized);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("No token provided."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return false;
  }

  std::string secret = "secret";
  
  try {
    MiddlewareUserdata* data = (MiddlewareUserdata*) userdata;
    auto decoded = jwt::decode(token);
    auto payload = decoded.get_payload_claim("username");
    std::string userId;
    std::string username = payload.as_string();

    DB* queryUserRole = DB::prepare("SELECT user_id, role_id FROM user_roles WHERE user_id = ':id'");
    std::string query = DB::prepare("SELECT id, name, username FROM users WHERE username = ':username'")
      ->bind(":username", username)
      ->get();

    auto result = Connection::getInstance().execute(query);
    if (result.empty()) {
      res->result(boost::beast::http::status::not_found);
      json_object* response = json_object_new_object();
      json_object_object_add(response, "error", json_object_new_string("User not found."));
      res->body() = json_object_to_json_string(response);
      res->prepare_payload();
      return false;
    }

    userId = result[0]["id"].c_str();
    std::string queryRole = queryUserRole->bind(":id", userId)->get();
    auto roleResult = Connection::getInstance().execute(queryRole);
    if (roleResult.empty()) {
      res->result(boost::beast::http::status::forbidden);
      json_object* response = json_object_new_object();
      json_object_object_add(response, "error", json_object_new_string("Access denied."));
      res->body() = json_object_to_json_string(response);
      res->prepare_payload();
      return false;
    }

  } catch (const std::exception& e) {
    res->result(boost::beast::http::status::unauthorized);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Invalid token."));
    json_object_object_add(response, "message", json_object_new_string(e.what()));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return false;
  }

  return true;
}