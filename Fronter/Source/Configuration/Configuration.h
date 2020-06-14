#ifndef CONFIGURATION
#define CONFIGURATION
#include "Log.h"
#include "Mod.h"
#include "Options/Option.h"
#include "Parser.h"
#include "RequiredFile.h"
#include "RequiredFolder.h"

class Configuration: commonItems::parser
{
  public:
	Configuration();
	explicit Configuration(std::istream& theStream);

	[[nodiscard]] const auto& getSourceGame() const { return sourceGame; }
	[[nodiscard]] const auto& getTargetGame() const { return targetGame; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getAutoGenerateModsFrom() const { return autoGenerateModsFrom; }
	[[nodiscard]] const auto& getAutoLocatedMods() const { return autolocatedMods; }
	[[nodiscard]] const auto& getPreloadedModFileNames() const { return preloadedModFileNames; }
	[[nodiscard]] const auto& getRequiredFiles() const { return requiredFiles; }
	[[nodiscard]] const auto& getRequiredFolders() const { return requiredFolders; }
	[[nodiscard]] const auto& getConverterFolder() const { return converterFolder; }
	[[nodiscard]] const auto& getOptions() const { return options; }

	[[nodiscard]] std::string getSecondTailSource() const;
	[[nodiscard]] bool exportConfiguration() const;
	void clearSecondLog() const;
	void autoLocateMods();

	void setEnabledMods(const std::set<int>& selection);

  private:
	void registerKeys();
	void registerPreloadKeys();

	std::string name;
	std::string converterFolder;
	std::string displayName;
	std::string sourceGame;
	std::string targetGame;
	std::string autoGenerateModsFrom;
	std::map<std::string, std::shared_ptr<RequiredFile>> requiredFiles;
	std::map<std::string, std::shared_ptr<RequiredFolder>> requiredFolders;
	std::vector<std::shared_ptr<Option>> options;
	std::vector<Mod> autolocatedMods;
	std::set<std::string> preloadedModFileNames;
	int optionCounter = 0;
};

#endif // CONFIGURATION
