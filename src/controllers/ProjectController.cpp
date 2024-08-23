#include <controllers/ProjectController.h>

void ProjectController::createProject(const Request& req, Response* res) {
  json_object* body = json_tokener_parse(req.body().c_str());
  json_object* response = json_object_new_object();
  std::string name = RequestHelper::getJsonSafeString(body, "name");
  std::string description = RequestHelper::getJsonSafeString(body, "description");
  std::string type = RequestHelper::getJsonSafeString(body, "type");

  try {
    std::string queryCheck = DB::prepare("SELECT * FROM projects WHERE name = ':name'")
      ->bind(":name", name)
      ->get();

    auto resultCheck = Connection::getInstance().execute(queryCheck);
    if (resultCheck.size() > 0) {
      throw HttpException(400, "Project name already exists.");
    }

    std::string query = DB::prepare("INSERT INTO projects (name, description, type) VALUES (':name', ':description', ':type') RETURNING id;")
      ->bind(":name", name)
      ->bind(":description", description)
      ->bind(":type", type)
      ->get();
      
    auto result = Connection::getInstance().execute(query);
    if (result.size() == 0) {
      throw HttpException(500, "An error occurred while creating the project.");
    }

  } catch (const HttpException& e) {
    res->result(boost::beast::http::int_to_status(e.code()));
    json_object_object_add(response, "code", json_object_new_int(e.code()));
    json_object_object_add(response, "error", json_object_new_string("An error occurred while creating the permission."));
    json_object_object_add(response, "message", json_object_new_string(e.what()));
    res->body() = json_object_to_json_string(response);
    res->prepare_payload();
    return;
  }

  res->result(boost::beast::http::status::ok);
  json_object_object_add(response, "message", json_object_new_string("Project created successfully."));
  res->body() = json_object_to_json_string(response);
  res->prepare_payload();
}
