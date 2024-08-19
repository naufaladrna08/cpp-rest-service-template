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
#include <middleware/JwtAuth.h>

namespace net = boost::asio;
using tcp = net::ip::tcp;

typedef struct {
  std::string method;
  std::string path;
  std::function<void(const Request&, Response*)> controller;
  std::function<bool(const Request&, Response*)> middleware;
} Route;

class Router {
  public:
    Router(Request* req, Response* res) {
      this->m_req = req;
      this->m_res = res;
    }

    void connect(const std::string& method, const std::string& path, std::function<void(const Request&, Response*)> controller, std::function<bool(const Request&, Response*)> middleware = nullptr) {
      Route route;
      route.method = method;
      route.path = path;
      route.controller = std::move(controller);
      route.middleware = std::move(middleware);

      m_routes.push_back(route);
    }

    void bind() {
      for (const auto& route : m_routes) {
        if (route.method == m_req->method_string() && route.path == m_req->target()) {
          if (route.middleware != nullptr) {
            if (!route.middleware(*m_req, m_res)) {
              return;
            }
          }

          route.controller(*m_req, m_res);
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
  res.version(req.version());
  res.result(boost::beast::http::status::ok);
  res.set(boost::beast::http::field::server, "Boost.Beast Server");
  res.set(boost::beast::http::field::content_type, "application/json");

  // OPTIONS CORS
  if (req.method() == boost::beast::http::verb::options) {
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::access_control_allow_methods, "GET, POST, OPTIONS");
    res.set(boost::beast::http::field::access_control_allow_headers, "Content-Type");
    res.set(boost::beast::http::field::access_control_max_age, "86400");
    return;
  }

  Router route(&req, &res);
  route.connect("POST", "/register", UserController::registerUser);
  route.connect("POST", "/login", UserController::login);
  route.connect("GET", "/users", UserController::getAllUsers);
  route.connect("GET", "/profile", UserController::profile, JwtAuth::verifyToken);

  App app;
  app.useRoute(route);
}

int main(int argc, char** argv) {
  try {
    dotenv::env.load_dotenv(".env");

    /* Initialize DB Connection */
    const std::string connectionStr = "dbname=" + dotenv::env["DB_NAME"] + " user=" + dotenv::env["DB_USER"] + " password=" + dotenv::env["DB_PASS"] + " hostaddr=" + dotenv::env["DB_HOST"] + " port=" + dotenv::env["DB_PORT"];
    std::cout << "Connecting to database with connection string " << connectionStr << std::endl;
    Connection::getInstance(connectionStr);

    net::io_context ioc;
    tcp::acceptor acceptor(ioc, {tcp::v4(), 8081});

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
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
