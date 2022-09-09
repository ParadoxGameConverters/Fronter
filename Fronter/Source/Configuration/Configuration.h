#ifndef CONFIGURATION
#define CONFIGURATION
#include "FronterMod.h"
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
	[[nodiscard]] auto getEnableUpdateChecker() const { return enableUpdateChecker; }
	[[nodiscard]] auto getCheckForUpdatesOnStartup() const { return checkForUpdatesOnStartup; }
	[[nodiscard]] const auto& getConverterReleaseForumThread() const { return converterReleaseForumThread; }
	[[nodiscard]] const auto& getLatestGitHubConverterReleaseUrl() const { return latestGitHubConverterReleaseUrl; }
	[[nodiscard]] const auto& getPagesCommitIdUrl() const { return pagesCommitIdUrl; }
	[[nodiscard]] const auto& getPreloadedModFileNames() const { return preloadedModFileNames; }
	[[nodiscard]] const auto& getRequiredFiles() const { return requiredFiles; }
	[[nodiscard]] const auto& getRequiredFolders() const { return requiredFolders; }
	[[nodiscard]] const auto& getConverterFolder() const { return converterFolder; }
	[[nodiscard]] const auto& getBackendExePath() const { return backendExePath; }
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
	std::string backendExePath; // relative to converterFolder
	std::string displayName;
	std::string sourceGame;
	std::string targetGame;
	std::string autoGenerateModsFrom;
	bool enableUpdateChecker = false;
	bool checkForUpdatesOnStartup = false;
	std::string converterReleaseForumThread;
	std::string latestGitHubConverterReleaseUrl;
	std::string pagesCommitIdUrl;
	std::map<std::string, std::shared_ptr<RequiredFile>> requiredFiles;
	std::map<std::string, std::shared_ptr<RequiredFolder>> requiredFolders;
	std::vector<std::shared_ptr<Option>> options;
	std::vector<FronterMod> autolocatedMods;
	std::set<std::string> preloadedModFileNames;
	int optionCounter = 0;
};

#endif // CONFIGURATION
