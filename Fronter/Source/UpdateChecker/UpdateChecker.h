#ifndef UPDATE_CHECKER
#define UPDATE_CHECKER

#include <string>
#include <curl/curl.h>

static int writer(char* data, size_t size, size_t nmemb, std::string* writerData);
static bool init(CURL*& conn, char* url);
bool isUpdateAvailable(const std::string& versionFilePath, const std::string& tagsUrl);

#endif