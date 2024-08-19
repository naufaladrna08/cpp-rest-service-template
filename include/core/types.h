#ifndef TYPES_H
#define TYPES_H
#include <boost/beast/http.hpp>

typedef boost::beast::http::request<boost::beast::http::string_body> Request;
typedef boost::beast::http::response<boost::beast::http::string_body> Response;

#endif //TYPES_H
