#include <controllers/UserController.h>

void UserController::login(Request req, Response *res) {
  json_object* body = json_tokener_parse(req.body().c_str());
  const char* username = json_object_get_string(json_object_object_get(body, "username"));
  const char* password = json_object_get_string(json_object_object_get(body, "password"));

  std::string query = DB::prepare("SELECT * FROM users WHERE username = ':username'")
    ->bind(":username", username)
    ->get();

  auto result = Connection::getInstance().execute(query);
  if (result.empty()) {
    res->result(boost::beast::http::status::unauthorized);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Invalid username or password."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  std::string passwordHash = result.at(0)["password"].c_str();
  if (!bcrypt::validatePassword(password, passwordHash)) {
    res->result(boost::beast::http::status::unauthorized);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Invalid username or password."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  res->result(boost::beast::http::status::ok);

  /* JWT Token */
  auto token = jwt::create()
    .set_issuer("auth0")
    .set_type("JWS")
    .set_payload_claim("username", jwt::claim(std::string(username)))
    .sign(jwt::algorithm::hs256{"secret"});

  /* Create Data */
  json_object* payload = json_object_new_object();
  json_object_object_add(payload, "username", json_object_new_string(username));
  json_object_object_add(payload, "token", json_object_new_string(token.c_str()));
  

  json_object* response = json_object_new_object();
  json_object_object_add(response, "message", json_object_new_string("Login successful."));
  json_object_object_add(response, "data", payload);
  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}

void UserController::registerUser(Request req, Response *res) {
  json_object* body = json_tokener_parse(req.body().c_str());
  const char* name = json_object_get_string(json_object_object_get(body, "name"));
  const char* username = json_object_get_string(json_object_object_get(body, "username"));
  const char* password = json_object_get_string(json_object_object_get(body, "password"));
  const char* email = json_object_get_string(json_object_object_get(body, "email"));
  std::string passwordHash = bcrypt::generateHash(password);

  std::string checkQuery = DB::prepare("SELECT * FROM users WHERE username = ':username' OR email = ':email'")
    ->bind(":username", username)
    ->bind(":email", email)
    ->get();

  auto checkResult = Connection::getInstance().execute(checkQuery);
  if (!checkResult.empty()) {
    res->result(boost::beast::http::status::conflict);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Username or email already exists."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  std::string query = DB::prepare("INSERT INTO users (name, username, password, email, created_at) VALUES (':name', ':username', ':password', ':email', NOW())")
    ->bind(":name", name)
    ->bind(":username", username)
    ->bind(":password", passwordHash.c_str())
    ->bind(":email", email)
    ->get(); 

  auto result = Connection::getInstance().execute(query);
  if (result.empty()) {
    res->result(boost::beast::http::status::ok);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "message", json_object_new_string("User registered successfully."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
  } else {
    res->result(boost::beast::http::status::internal_server_error);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("An error occurred while registering the user."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
  }
}

void UserController::getAllUsers(Request req, Response *res) {
  std::string selectQuery = "SELECT name, username FROM users";
  auto result = Connection::getInstance().execute(selectQuery);

  json_object* response = json_object_new_array();
  for (auto row : result) {
    json_object* user = json_object_new_object();
    json_object_object_add(user, "name", json_object_new_string(row["name"].c_str()));
    json_object_object_add(user, "username", json_object_new_string(row["username"].c_str()));
    json_object_array_add(response, user);
  }

  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}

void UserController::profile(Request req, Response *res) {
  std::string token = req.base().at("Authorization");
  auto decoded = jwt::decode(token);
  auto payload = decoded.get_payload_claim("username");
  std::string username = payload.as_string();

  std::string query = DB::prepare("SELECT * FROM users WHERE username = ':username'")
    ->bind(":username", username)
    ->get();

  auto result = Connection::getInstance().execute(query);
  if (result.empty()) {
    res->result(boost::beast::http::status::not_found);
    json_object* response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("User not found."));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  json_object* user = json_object_new_object();
  json_object_object_add(user, "name", json_object_new_string(result.at(0)["name"].c_str()));
  json_object_object_add(user, "username", json_object_new_string(result.at(0)["username"].c_str()));
  json_object_object_add(user, "email", json_object_new_string(result.at(0)["email"].c_str()));

  json_object* response = json_object_new_object();
  json_object_object_add(response, "code", json_object_new_int(200));
  json_object_object_add(response, "data", user);
  json_object_object_add(response, "message", json_object_new_string("User profile retrieved successfully."));
  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}