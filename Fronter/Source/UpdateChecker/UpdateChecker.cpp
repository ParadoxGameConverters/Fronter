#include "UpdateChecker.h"
#include "ParserHelpers.h"
#include "Log.h"
#include <set>
#include <fstream>



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

	Log(LogLevel::Info) << localCommitId << " " << localCommitId.size();
	Log(LogLevel::Info) << latestReleaseCommitId << " " << latestReleaseCommitId.size();

	if (localCommitId != latestReleaseCommitId)
	{
		return true;
	}
	return false;
}
