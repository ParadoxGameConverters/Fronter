#include "OSFunctions.h"
#include "OSCompatibilityLayer.h"
#include <codecvt>
#include <filesystem>
namespace fs = std::filesystem;

std::string UTF16ToUTF8(const std::wstring& theString)
{
	std::u16string u16str(theString.begin(), theString.end());
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
	return conversion.to_bytes(u16str);
}

std::optional<std::string> getSteamInstallPath(const std::string& steamID)
{
	return std::nullopt;
}
