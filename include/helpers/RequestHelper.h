#ifndef REQUEST_HELPER_H
#define REQUEST_HELPER_H
#include <string>
#include <core/types.h>
#include <json.h>

class RequestHelper {
  public:
    static std::string getJsonSafeString(json_object* body, const std::string& key);
    static int getJsonSafeInt(json_object* body, const std::string& key);
    static bool getJsonSafeBool(json_object* body, const std::string& key);
    static double getJsonSafeDouble(json_object* body, const std::string& key);
    static json_object* getJsonSafeObject(json_object* body, const std::string& key);
    static json_object* getJsonSafeArray(json_object* body, const std::string& key);
    static json_object* getJsonSafeArrayElement(json_object* body, const std::string& key, int index);
    static bool isJsonKeyExists(json_object* body, const std::string& key);
    static bool isJsonKeyNull(json_object* body, const std::string& key);
};

#endif