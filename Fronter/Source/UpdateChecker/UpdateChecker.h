#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H



#include <filesystem>
#include <optional>
#include <string>



struct UpdateInfo
{
	std::string version;
	std::string description;
	std::optional<std::string> zipURL;
};

bool isUpdateAvailable(const std::filesystem::path& commitIdFilePath, const std::string& commitIdURL);
UpdateInfo getLatestReleaseInfo(const std::string& converterName);
std::wstring getUpdateMessageBody(const std::wstring& baseBody, const UpdateInfo& updateInfo);

#endif // UPDATE_CHECKER_H