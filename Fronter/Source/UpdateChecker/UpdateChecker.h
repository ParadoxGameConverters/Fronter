#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H


#include <curl/curl.h>
#include <string>


struct UpdateInfo
{
	std::string version;
	std::string description;
};


bool isUpdateAvailable(const std::string& commitIdFilePath, const std::string& commitIdURL);
UpdateInfo getLatestReleaseInfo(const std::string& converterName);
std::wstring getUpdateMessageBody(const std::wstring& baseBody, const std::string& converterName);


#endif // UPDATE_CHECKER_H