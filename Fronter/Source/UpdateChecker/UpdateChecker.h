#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H


#include <curl/curl.h>
#include <string>


bool isUpdateAvailable(const std::string& commitIdFilePath, const std::string& commitIdURL);
std::string getLatestReleaseDescription(const std::string& converterName);
std::wstring getUpdateMessageBody(const std::wstring& baseBody, const std::string& converterName);


#endif // UPDATE_CHECKER_H