#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H


#include <curl/curl.h>
#include <string>


static int writer(char* data, size_t size, size_t nmemb, std::string* writerData);
static bool init(CURL*& conn, char* url);
bool isUpdateAvailable(const std::string& versionFilePath, const std::string& tagsUrl);

#endif