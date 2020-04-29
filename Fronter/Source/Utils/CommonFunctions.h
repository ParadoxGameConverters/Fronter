#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS
#include <string>
#include "Log.h"

enum class MessageSource
{
	UNINITIALIZED = 0,
	UI = 1,
	CONVERTER = 2
};

typedef struct
{
	std::string timestamp;
	LogLevel logLevel = LogLevel::Info;
	MessageSource source = MessageSource::UNINITIALIZED;
	std::string message;
} LogMessage;

LogMessage sliceMessage(const std::string& message);
std::string normalizeStringPath(const std::string& stringPath);
std::string replaceCharacter(std::string fileName, char character);
std::string trimPath(const std::string& fileName);

#endif // COMMON_FUNCTIONS
