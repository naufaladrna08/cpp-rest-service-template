#include <controllers/UserController.h>

UserController::UserController() {
}

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

void UserController::registerUser(Request req, Response* res) {
  json_object* body = json_tokener_parse(req.body().c_str());
  std::string name = RequestHelper::getJsonSafeString(body, "name");
  std::string username = RequestHelper::getJsonSafeString(body, "username");
  std::string password = RequestHelper::getJsonSafeString(body, "password");

  json_object* response = json_object_new_object();
  try {
    // std::unique_ptr<User> user = std::make_unique<User>();
    // std::unique_ptr<User> existingUser = user->findByValue<User>("username", username);
    // if (existingUser != nullptr) {
    //   throw HttpException(400, "Username already exists.");
    // }

    // user->fill("name", name);
    // user->fill("username", username);
    // user->fill("password", bcrypt::generateHash(password));
    // if (!user->save()) {
    //   throw HttpException(500, "An error occurred while creating the user.");
    // }

    std::unique_ptr<odb::core::database> db(new odb::pgsql::database("postgres", "12341234", "app", "localhost"));
    odb::core::transaction t(db->begin());
    users u(name, username, bcrypt::generateHash(password));
    db->persist(u);
    t.commit();

    res->result(boost::beast::http::status::ok);
    json_object_object_add(response, "message", json_object_new_string("User created successfully."));
  } catch (const HttpException& e) {
    res->result(boost::beast::http::int_to_status(e.code()));
    json_object_object_add(response, "message", json_object_new_string(e.what()));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}

void UserController::getAllUsers(Request req, Response *res) {
  json_object* response = json_object_new_object();
  
  try {
    auto users = User().select({"name", "username", "email", "user_roles.id AS role_id"})
      ->join("user_roles", "id", "user_id")
      ->get();
    
    if (users.empty()) {
      throw HttpException(404, "No users found.");
    }

    json_object* data = json_object_new_array();
    for (const auto& user : users) {
      json_object* userObj = json_object_new_object();
      json_object_object_add(userObj, "name", json_object_new_string(user["name"].c_str()));
      json_object_object_add(userObj, "username", json_object_new_string(user["username"].c_str()));
      json_object_object_add(userObj, "email", json_object_new_string(user["email"].c_str()));
      json_object_object_add(userObj, "role_id", json_object_new_int(std::stoi(user["role_id"].c_str())));
      json_object_array_add(data, userObj);
    }

    json_object_object_add(response, "code", json_object_new_int(200));
    json_object_object_add(response, "data", data);
    json_object_object_add(response, "message", json_object_new_string("Users retrieved successfully."));
  } catch (const HttpException& e) {
    res->result(boost::beast::http::int_to_status(e.code()));
    json_object_object_add(response, "error", json_object_new_string(e.what()));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
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

std::unique_ptr<odb::database> UserController::createDatabase()  {
  std::unique_ptr<odb::database> db(new odb::pgsql::database("postgres", "12341234", "app"));
  return db;
}