#include <helpers/RequestHelper.h>

std::string RequestHelper::getJsonSafeString(json_object* body, const std::string& key) {
  if (json_object_object_get(body, key.c_str()) == nullptr) {
    return "";
  }

  return json_object_get_string(json_object_object_get(body, key.c_str()));
}

int RequestHelper::getJsonSafeInt(json_object* body, const std::string& key) {
  if (json_object_object_get(body, key.c_str()) == nullptr) {
    return 0;
  }

  return json_object_get_int(json_object_object_get(body, key.c_str()));
}

bool RequestHelper::getJsonSafeBool(json_object* body, const std::string& key) {
  if (json_object_object_get(body, key.c_str()) == nullptr) {
    return false;
  }

  return json_object_get_boolean(json_object_object_get(body, key.c_str()));
}

double RequestHelper::getJsonSafeDouble(json_object* body, const std::string& key) {
  if (json_object_object_get(body, key.c_str()) == nullptr) {
    return 0.0;
  }

  return json_object_get_double(json_object_object_get(body, key.c_str()));
}

json_object* RequestHelper::getJsonSafeObject(json_object* body, const std::string& key) {
  if (json_object_object_get(body, key.c_str()) == nullptr) {
    return nullptr;
  }

  return json_object_object_get(body, key.c_str());
}

json_object* RequestHelper::getJsonSafeArray(json_object* body, const std::string& key) {
  if (json_object_object_get(body, key.c_str()) == nullptr) {
    return nullptr;
  }

  return json_object_object_get(body, key.c_str());
}

json_object* RequestHelper::getJsonSafeArrayElement(json_object* body, const std::string& key, int index) {
  if (json_object_object_get(body, key.c_str()) == nullptr) {
    return nullptr;
  }

  json_object* array = json_object_object_get(body, key.c_str());
  if (json_object_get_type(array) != json_type_array) {
    return nullptr;
  }

  return json_object_array_get_idx(array, index);
}

bool isJsonKeyExists(json_object* body, const std::string& key) {
  return json_object_object_get(body, key.c_str()) != nullptr;
}

bool isJsonKeyNull(json_object* body, const std::string& key) {
  return json_object_object_get(body, key.c_str()) == nullptr;
}