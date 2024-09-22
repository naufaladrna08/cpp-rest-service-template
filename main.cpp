#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <json.h>

#include <core/types.h>
#include <dotenv.h>
#include <core/Connection.h>

#include <controllers/UserController.h>
#include <controllers/ProjectController.h>
#include <controllers/RBACController.h>
#include <middleware/JwtAuth.h>
#define APP_PORT 8080

namespace net = boost::asio;
using tcp = net::ip::tcp;

typedef struct {
  std::string method;
  std::string path;
  std::function<void(const Request&, Response*)> controller;
  std::function<bool(const Request&, Response*, void* userdata)> middleware;
  void* userdata;
} Route;

class Router {
  public:
    Router(Request* req, Response* res) {
      this->m_req = req;
      this->m_res = res;
    }

    void connect(
      const std::string& method, const std::string& path, 
      std::function<void(const Request&, Response*)> controller, 
      std::function<bool(const Request&, Response*, void* userdata)> middleware = nullptr,
      void* data = nullptr
    ) {
      Route route;
      route.method = method;
      route.path = path;
      route.controller = std::move(controller);
      route.middleware = std::move(middleware);
      
      if (data != nullptr) {
        route.userdata = data;
      }

      m_routes.push_back(route);
    }

    void bind() {
      for (const auto& route : m_routes) {
        if (route.method == m_req->method_string() && route.path == m_req->target()) {
          if (route.middleware != nullptr) {
            if (!route.middleware(*m_req, m_res, route.userdata)) {
              return;
            }
          }

          /* For syntax error, etc/ */
          try {
            route.controller(*m_req, m_res);
          } catch (const std::exception& e) {
            m_res->result(boost::beast::http::status::internal_server_error);
            json_object* response = json_object_new_object();
            json_object_object_add(response, "error", json_object_new_string(e.what()));
            m_res->body() = json_object_to_json_string(response);
            m_res->prepare_payload();
          }
          return;
        }
      }

      // If no route matches, return a 404 response
      m_res->result(boost::beast::http::status::not_found);
      json_object* response = json_object_new_object();
      json_object_object_add(response, "error", json_object_new_string("The requested resource could not be found."));
      m_res->body() = json_object_to_json_string(response);
      m_res->prepare_payload();
    }

  private:
    std::vector<Route> m_routes;

    Request* m_req;
    Response* m_res;
    void* m_data;
};

class App {
  public:
    explicit App() {
      this->m_router = nullptr;
    }

    void useRoute(const Router& router) {
      this->m_router = const_cast<Router*>(&router);
      this->m_router->bind();
    }

  private:
    Router* m_router;
};

void handleRequests(Request req, Response& res) {
  std::string allowedOrigin = dotenv::env["ALLOWED_ORIGIN"];

  res.version(req.version());
  res.result(boost::beast::http::status::ok);
  res.set(boost::beast::http::field::server, "Boost.Beast Server");
  res.set(boost::beast::http::field::content_type, "application/json");
  res.set(boost::beast::http::field::access_control_allow_origin, allowedOrigin);
  res.set(boost::beast::http::field::access_control_allow_methods, "GET, POST, OPTIONS");
  res.set(boost::beast::http::field::access_control_allow_headers, "Content-Type");
  res.set(boost::beast::http::field::access_control_max_age, "86400");

  Router route(&req, &res);
  route.connect("POST", "/auth/register", UserController::registerUser);
  route.connect("POST", "/auth/create-user", UserController::createUser);
  route.connect("POST", "/auth/login", UserController::login);
  route.connect("GET", "/auth/me", UserController::profile, JwtAuth::verifyToken);
  route.connect("GET", "/users", UserController::getAllUsers);

  // RBAC
  MiddlewareUserdata userdata;
  userdata.role = "Superuser";
  route.connect("POST", "/role/create", RBACController::createRole, JwtAuth::verifyTokenWithRole, &userdata);
  route.connect("POST", "/role/permission/create", RBACController::createPermission, JwtAuth::verifyTokenWithRole, &userdata);
  route.connect("POST", "/role/assign", RBACController::assignRole, JwtAuth::verifyTokenWithRole, &userdata);

   // Projects
  route.connect("POST", "/project/create", ProjectController::createProject, JwtAuth::verifyTokenWithRole, &userdata);
  // route.connect("GET", "/project/get-projects", ProjectController::getProjects, JwtAuth::verifyToken);
  // route.connect("GET", "/project/get-project", ProjectController::getProject, JwtAuth::verifyToken);
  // route.connect("PUT", "/project/update-project", ProjectController::updateProject, JwtAuth::verifyToken);
  // route.connect("DELETE", "/project/delete-project", ProjectController::deleteProject, JwtAuth::verifyToken);

  App app;
  app.useRoute(route);
}

int main(int argc, char** argv) {
  try {
    dotenv::env.load_dotenv(".env");

    /* Initialize DB Connection */
    const std::string connectionStr = "dbname=" + dotenv::env["DB_NAME"] + " user=" + dotenv::env["DB_USER"] + " password=" + dotenv::env["DB_PASS"] + " hostaddr=" + dotenv::env["DB_HOST"] + " port=" + dotenv::env["DB_PORT"];
    std::cout << "Running on port " << APP_PORT << std::endl;
    Connection::getInstance(connectionStr);

    net::io_context ioc;
    tcp::acceptor acceptor(ioc, {tcp::v4(), APP_PORT});

    while (true) {
      std::cout << "Waiting for request..." << std::endl;
      tcp::socket socket(ioc);
      acceptor.accept(socket);

      boost::beast::flat_buffer buffer;
      boost::beast::http::request<boost::beast::http::string_body> req;
      boost::beast::http::read(socket, buffer, req);

      boost::beast::http::response<boost::beast::http::string_body> res;
      handleRequests(req, res);

      boost::beast::http::write(socket, res);
      socket.shutdown(tcp::socket::shutdown_send);
    }
  }  catch (std::exception const &e) {
    std::cerr << "[EXCEPTION]: " << e.what() << std::endl;
    return 1;
  }
}
