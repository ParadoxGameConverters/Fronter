#ifndef OS_FUNCTIONS
#define OS_FUNCTIONS
#include <optional>
#include <string>

std::optional<std::string> getSteamInstallPath(const std::string& steamID);
std::string UTF16ToUTF8(const std::wstring& theString);

#endif // OS_FUNCTIONS
