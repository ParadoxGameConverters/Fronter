#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H


#include <curl/curl.h>
#include <string>


bool isUpdateAvailable(const std::string& versionFilePath, const std::string& tagsUrl);

#endif