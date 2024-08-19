#include <middleware/JwtAuth.h>

bool JwtAuth::verifyToken(const Request& req, Response* res) {
  std::string token = req.base()["Authorization"].to_string();
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
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return false;
  }

  return true;
}