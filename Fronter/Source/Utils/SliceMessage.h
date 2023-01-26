#ifndef SLICE_MESSAGE
#define SLICE_MESSAGE
#include "Log.h"
#include <string>

enum class MessageSource
{
	UNINITIALIZED = 0,
	UI = 1,
	CONVERTER = 2
};

typedef struct LogMessage
{
	std::string timestamp;
	LogLevel logLevel = LogLevel::Info;
	MessageSource source = MessageSource::UNINITIALIZED;
	std::string message;
} LogMessage;

LogMessage sliceMessage(const std::string& message);

#endif // SLICE_MESSAGE
