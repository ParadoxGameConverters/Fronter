#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H


#include <curl/curl.h>
#include <string>


bool isUpdateAvailable(const std::string& commitIdFilePath, const std::string& commitIdURL);


#endif // UPDATE_CHECKER_H