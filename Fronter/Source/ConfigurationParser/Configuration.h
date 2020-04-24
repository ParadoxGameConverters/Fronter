#ifndef CONFIGURATION
#define CONFIGURATION
#include "RequiredFile.h"
#include "RequiredFolder.h"
#include "newParser.h"

namespace Configuration
{
class Configuration: commonItems::parser
{
  public:
	Configuration();
	explicit Configuration(std::istream& theStream);

	[[nodiscard]] const auto& getSourceGame() const { return sourceGame; }
	[[nodiscard]] const auto& getTargetGame() const { return targetGame; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getOptionsFile() const { return optionsFile; }
	[[nodiscard]] const auto& getRequiredFiles() const { return requiredFiles; }
	[[nodiscard]] const auto& getRequiredFolders() const { return requiredFolders; }

  private:
	void registerKeys();
	std::string name;
	std::string converterFolder;
	std::string displayName;
	std::string optionsFile;
	std::string sourceGame;
	std::string targetGame;
	std::map<std::string, std::shared_ptr<RequiredFile>> requiredFiles;
	std::map<std::string, std::shared_ptr<RequiredFolder>> requiredFolders;
};
} // namespace Configuration

#endif // CONFIGURATION
