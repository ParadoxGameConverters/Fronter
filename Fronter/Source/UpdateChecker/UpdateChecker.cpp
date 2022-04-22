#include "UpdateChecker.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <algorithm>
#include <cctype>
#include <codecvt>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <wx/utils.h>

using json = nlohmann::json;


//
//  libcurl variables for error strings and returned data

static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

//
//  libcurl write callback function
//

static int writer(const char* data, const size_t size, const size_t nmemb, std::string* writerData)
{
	if (!writerData)
		return 0;

	writerData->append(data, size * nmemb);

	return size * nmemb;
}

//
//  libcurl connection initialization
//

static bool init(CURL*& conn, char* url)
{
	conn = curl_easy_init();

	if (conn == nullptr)
	{
		fprintf(stderr, "Failed to create CURL connection\n");
		exit(EXIT_FAILURE);
	}

	CURLcode code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set error buffer [%d]\n", code);
		return false;
	}

	//additional options
	code = curl_easy_setopt(conn, CURLOPT_SSL_VERIFYPEER, 0);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to disable verifying the peer in ssl connection [%d]\n", code);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_USERAGENT, "ParadoxGameConverters");
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set useragent [%d]\n", code);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_URL, url);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return false;
	}
	
	return true;
}

bool isUpdateAvailable(const std::string& commitIdFilePath, const std::string& commitIdURL)
{
	if (!commonItems::DoesFileExist(commitIdFilePath))
	{
		return false;
	}

	CURL* conn = nullptr;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Initialize CURL connection
	if (!init(conn, const_cast<char*>(commitIdURL.c_str())))
	{
		return false;
	}

	// Retrieve content for the URL
	const CURLcode code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if (code != CURLE_OK)
	{
		return false;
	}
	std::string latestReleaseCommitId = buffer;
	buffer.clear();
	// remove whitespace from the latest release commit string
	latestReleaseCommitId.erase(std::ranges::remove_if(latestReleaseCommitId,
	                                                   [](const unsigned char x) {
		                                                   return std::isspace(x);
	                                                   }).begin(),
								latestReleaseCommitId.end());

	std::ifstream commitIdFile(commitIdFilePath);
	std::string localCommitId;
	commitIdFile >> localCommitId;
	commitIdFile.close();

	if (localCommitId != latestReleaseCommitId)
	{
		return true;
	}
	return false;
}

UpdateInfo getLatestReleaseInfo(const std::string& converterName)
{
	UpdateInfo info;
	const auto apiUrl = "https://api.github.com/repos/ParadoxGameConverters/" + converterName + "/releases/latest";

	CURL* conn = nullptr;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Initialize CURL connection
	if (!init(conn, const_cast<char*>(apiUrl.c_str())))
	{
		return {};
	}

	// Retrieve content for the URL
	const CURLcode code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if (code != CURLE_OK)
	{
		return {};
	}
	std::string jsonResponse = buffer;
	buffer.clear();

	auto j = json::parse(jsonResponse);
	info.description = j["body"];
	info.version = j["name"];
	auto& assets = j["assets"];
	for (auto& asset : assets)
	{
		std::string assetName = asset["name"];
#ifdef _WIN32
		const std::string osName = "win";
#elif __linux__
		const std::string osName = "linux";
#elif __APPLE__
		const std::string osName = "osx";
#endif
		
		std::ranges::transform(assetName,
		                       assetName.begin(),
		                       [](const unsigned char c)
		                       {
			                       return std::tolower(c);
		                       });
		if (assetName.ends_with("-" + osName + "-x64.zip"))
		{
			info.zipURL = asset["browser_download_url"];
			break;
		}
	}
	if (!info.zipURL)
	{
		Log(LogLevel::Warning) << "Release " << info.version << " doesn't have a .zip asset.";
	}
	return info;
}

std::wstring getUpdateMessageBody(const std::wstring& baseBody, const UpdateInfo& updateInfo)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	const auto& [version, description, zipURL] = updateInfo;

	auto body = baseBody;
	body.append(converter.from_bytes("\n\n"));
	body.append(converter.from_bytes("Version: " + version));
	body.append(converter.from_bytes("\n\n"));
	body.append(converter.from_bytes(description));
	return body;
}

void startUpdaterAndDie(const std::string& zipURL, const std::string& converterBackendDirName)
{
	if (!commonItems::DeleteFolder("./Updater-running"))
	{
		return;
	}
	if (!commonItems::CopyFolder("./Updater", "./Updater-running"))
	{
		return;
	}
#ifdef _WIN32
	const std::wstring commandLineString = commonItems::convertUTF8ToUTF16(
		"./Updater-running/updater.exe " + zipURL + " " + converterBackendDirName
	);
#else
	const std::wstring commandLineString = commonItems::convertUTF8ToUTF16(
		"./Updater-running/updater " + zipURL + " " + converterBackendDirName
	);
#endif

	wxExecute(commandLineString, wxEXEC_SHOW_CONSOLE);

	// Die (the updater will start Fronter after a successful update)
	exit(0);
}
