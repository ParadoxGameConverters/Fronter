#ifndef CONFIGURATION
#define CONFIGURATION
#include "Log.h"
#include "Options/Option.h"
#include "RequiredFile.h"
#include "RequiredFolder.h"
#include "newParser.h"

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

class Configuration: commonItems::parser
{
  public:
	Configuration();
	explicit Configuration(std::istream& theStream);

	[[nodiscard]] const auto& getSourceGame() const { return sourceGame; }
	[[nodiscard]] const auto& getTargetGame() const { return targetGame; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getRequiredFiles() const { return requiredFiles; }
	[[nodiscard]] const auto& getRequiredFolders() const { return requiredFolders; }
	[[nodiscard]] const auto& getOptions() const { return options; }

	[[nodiscard]] std::string getSecondTailSource() const;
	[[nodiscard]] bool exportConfiguration() const;
	[[nodiscard]] bool copyMod() const;

	static LogMessage sliceMessage(const std::string& message);
	static std::string normalizeStringPath(const std::string& stringPath);
	static std::string replaceCharacter(std::string fileName, char character);
	static std::string trimPath(const std::string& fileName);

  private:
	void registerKeys();
	void registerPreloadKeys();
	std::string name;
	std::string converterFolder;
	std::string displayName;
	std::string sourceGame;
	std::string targetGame;
	std::map<std::string, std::shared_ptr<RequiredFile>> requiredFiles;
	std::map<std::string, std::shared_ptr<RequiredFolder>> requiredFolders;
	std::vector<std::shared_ptr<Option>> options;
	int optionCounter = 0;
};

#endif // CONFIGURATION
