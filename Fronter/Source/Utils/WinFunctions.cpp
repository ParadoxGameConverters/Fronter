#include "OSFunctions.h"
#include "OSCompatibilityLayer.h"
#include <windows.h> // Include windows BEFORE other windows-related includes!
#include <codecvt>
#include <filesystem>
#include <winreg.h>
namespace fs = std::filesystem;

std::string UTF16ToUTF8(const std::wstring& theString)
{
	std::u16string u16str(theString.begin(), theString.end());
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
	return conversion.to_bytes(u16str);
}

std::optional<std::string> getSteamInstallPath(const std::string& steamID)
{
	if (steamID.empty())
		return std::nullopt;

	wchar_t value[255];
	value[0] = 0;
	DWORD BufferSize = 8192;
	std::wstring registryPath = Utils::convertUTF8ToUTF16(R"(SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	const std::wstring installPath = Utils::convertUTF8ToUTF16(R"(InstallLocation)");

	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, (PVOID)&value, &BufferSize);

	if (value[0] != 0)
	{
		auto result = Utils::convertUTF16ToUTF8(std::wstring(value));
		if (result.length() > 2)
		{
			return result;
		}
	}

	registryPath = Utils::convertUTF8ToUTF16(R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, (PVOID)&value, &BufferSize);

	if (value[0] != 0)
	{
		auto result = Utils::convertUTF16ToUTF8(std::wstring(value));
		if (result.length() > 2)
		{
			return result;
		}
	}

	return std::nullopt;
}
