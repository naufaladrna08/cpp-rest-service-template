#include <ws.h>
#include <json.h>
#include <postgresql/libpq-fe.h>

ws_vector_t* json_headers() {
  ws_vector_t* headers = ws_vector_create();
  ws_vector_push(headers, "Content-Type: application/json");
  return headers;
}

int hello_handler(int client_soc, const char* body) {
  ws_make_response_with_headers(client_soc, 200, "{\"message\": \"Hello, World!\"}", json_headers());
  return 0;
}

int main(int argc, char** argv) {
  ws_t *ws = ws_create(8080);
  if (!ws) {
    return -1;
  }

  ws_add_route(ws, "/hello", hello_handler);
  ws_pool(ws);

  ws_clear_routes(ws);
  ws_destroy(ws);
  return 0;
}