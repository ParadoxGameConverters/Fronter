#include "UpdateChecker.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>
#include <codecvt>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


//
//  libcurl variables for error strings and returned data

static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

//
//  libcurl write callback function
//

static int writer(char* data, size_t size, size_t nmemb, std::string* writerData)
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
	CURLcode code;

	conn = curl_easy_init();

	if (conn == NULL)
	{
		fprintf(stderr, "Failed to create CURL connection\n");
		exit(EXIT_FAILURE);
	}

	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
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
	CURLcode code;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Initialize CURL connection
	if (!init(conn, const_cast<char*>(commitIdURL.c_str())))
	{
		return false;
	}

	// Retrieve content for the URL
	code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if (code != CURLE_OK)
	{
		return false;
	}
	std::string latestReleaseCommitId = buffer;
	buffer.clear();
	// remove whitespace from the latest release commit string
	latestReleaseCommitId.erase(std::remove_if(latestReleaseCommitId.begin(),
											   latestReleaseCommitId.end(),
											   [](unsigned char x) {
												   return std::isspace(x);
											   }),
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
	auto apiUrl = "https://api.github.com/repos/ParadoxGameConverters/" + converterName + "/releases/latest";

	CURL* conn = nullptr;
	CURLcode code;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Initialize CURL connection
	if (!init(conn, const_cast<char*>(apiUrl.c_str())))
	{
		return {};
	}

	// Retrieve content for the URL
	code = curl_easy_perform(conn);
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
	return info;
}

std::wstring getUpdateMessageBody(const std::wstring& baseBody, const std::string& converterName)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	auto info = getLatestReleaseInfo(converterName);

	auto body = baseBody;
	body.append(converter.from_bytes("\n\n"));
	body.append(converter.from_bytes("Version: " + info.version));
	body.append(converter.from_bytes("\n\n"));
	body.append(converter.from_bytes(info.description));
	return body;
}
