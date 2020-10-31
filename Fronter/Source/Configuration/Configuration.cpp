#include "Configuration.h"
#include "Mod.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

Configuration::Configuration()
{
	std::ofstream clearLog("log.txt");
	clearLog.close();
	registerKeys();
	if (fs::exists("Configuration/fronter-configuration.txt"))
	{
		parseFile("Configuration/fronter-configuration.txt");
		Log(LogLevel::Info) << "Frontend configuration loaded.";
	}
	else
	{
		Log(LogLevel::Warning) << "Configuration/fronter-configuration.txt not found!";
	}
	if (fs::exists("Configuration/fronter-options.txt"))
	{
		parseFile("Configuration/fronter-options.txt");
		Log(LogLevel::Info) << "Frontend options loaded.";
	}
	else
	{
		Log(LogLevel::Warning) << "Configuration/fronter-options.txt not found!";
	}
	clearRegisteredKeywords();
	registerPreloadKeys();
	if (!converterFolder.empty() && fs::exists(fs::u8path(converterFolder + "/configuration.txt")))
	{
		Log(LogLevel::Info) << "Previous configuration located, preloading selections.";
		parseFile(converterFolder + "/configuration.txt");
	}
	clearRegisteredKeywords();
}

void Configuration::registerPreloadKeys()
{
	registerRegex("[a-zA-Z0-9_-]+", [this](const std::string& incomingKey, std::istream& theStream) {
		const commonItems::stringOfItem valueStr(theStream);
		std::stringstream ss;
		ss.str(valueStr.getString());
		const commonItems::singleString theString(ss);

		if (incomingKey == "configuration")
		{
			Log(LogLevel::Warning) << "You have an old configuration file. Preload will not be possible.";
			return;
		}
		for (const auto& folder: requiredFolders)
		{
			if (folder.first == incomingKey)
				folder.second->setValue(theString.getString());
		}
		for (const auto& file: requiredFiles)
		{
			if (file.first == incomingKey)
				file.second->setValue(theString.getString());
		}
		for (const auto& option: options)
		{
			if (option->getName() == incomingKey && !option->getCheckBoxSelector().first)
			{
				option->setValue(theString.getString());
			}
			else if (option->getName() == incomingKey && option->getCheckBoxSelector().first)
			{
				commonItems::stringList theList(ss);
				const auto selections = theList.getStrings();
				std::set<std::string> values = std::set(selections.begin(), selections.end());
				option->setValue(values);
				option->setCheckBoxSelectorPreloaded();
			}
		}
		if (incomingKey == "selectedMods")
		{
			commonItems::stringList theList(ss);
			const auto selections = theList.getStrings();
			preloadedModFileNames.insert(selections.begin(), selections.end());
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

Configuration::Configuration(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void Configuration::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("converterFolder", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		converterFolder = nameStr.getString();
	});
	registerKeyword("requiredFolder", [this](const std::string& unused, std::istream& theStream) {
		auto newFolder = std::make_shared<RequiredFolder>(theStream);
		if (!newFolder->getName().empty())
			requiredFolders.insert(std::pair(newFolder->getName(), newFolder));
		else
			Log(LogLevel::Error) << "Required Folder has no mandatory field: name!";
	});
	registerKeyword("requiredFile", [this](const std::string& unused, std::istream& theStream) {
		auto newFile = std::make_shared<RequiredFile>(theStream);
		if (!newFile->getName().empty())
			requiredFiles.insert(std::pair(newFile->getName(), newFile));
		else
			Log(LogLevel::Error) << "Required File has no mandatory field: name!";
	});
	registerKeyword("option", [this](const std::string& unused, std::istream& theStream) {
		optionCounter++;
		auto newOption = std::make_shared<Option>(theStream, optionCounter);
		options.emplace_back(newOption);
	});
	registerKeyword("displayName", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		displayName = nameStr.getString();
	});
	registerKeyword("sourceGame", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString gameStr(theStream);
		sourceGame = gameStr.getString();
	});
	registerKeyword("targetGame", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString gameStr(theStream);
		targetGame = gameStr.getString();
	});
	registerKeyword("autoGenerateModsFrom", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString modsStr(theStream);
		autoGenerateModsFrom = modsStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

bool Configuration::exportConfiguration() const
{
	if (converterFolder.empty())
	{
		Log(LogLevel::Error) << "Converter folder is not set!";
		return false;
	}
	if (!fs::exists(fs::u8path(converterFolder)))
	{
		Log(LogLevel::Error) << "Could not find converter folder!";
		return false;
	}
	std::ofstream confFile(fs::u8path(converterFolder + "/configuration.txt"));
	if (!confFile.is_open())
	{
		Log(LogLevel::Error) << "Could not open configuration.txt!";
		return false;
	}

	for (const auto& folder: requiredFolders)
	{
		confFile << folder.first << " = \"" << folder.second->getValue() << "\"\n";
	}
	for (const auto& file: requiredFiles)
	{
		if (!file.second->isOutputtable())
			continue;
		confFile << file.first << " = \"" << file.second->getValue() << "\"\n";
	}

	if (!autoGenerateModsFrom.empty())
	{
		confFile << "selectedMods = {\n";
		for (const auto& mod: autolocatedMods)
			if (preloadedModFileNames.count(mod.getFileName()))
			{
				confFile << "\t\"" << mod.getFileName() << "\"\n";
			}
		confFile << "}\n";
	}

	for (const auto& option: options)
	{
		if (option->getCheckBoxSelector().first)
		{
			confFile << option->getName() << " = { ";
			for (const auto& value: option->getValues())
			{
				confFile << "\"" << value << "\" ";
			}
			confFile << "}\n";
		}
		else
		{
			confFile << option->getName() << " = \"" << option->getValue() << "\"\n";
		}
	}
	confFile.close();
	return true;
}

std::string Configuration::getSecondTailSource() const
{
	return converterFolder + "/log.txt";
}

void Configuration::clearSecondLog() const
{
	std::ofstream clearSecondLog(converterFolder + "/log.txt");
	clearSecondLog.close();
}

void Configuration::autoLocateMods()
{
	autolocatedMods.clear();
	// Do we have a mod path?
	std::string modPath;
	for (const auto& requiredfolder: requiredFolders)
	{
		if (requiredfolder.second->getName() == autoGenerateModsFrom)
		{
			modPath = requiredfolder.second->getValue();
		}
	}
	if (modPath.empty())
		return;

	// Does it exist?
	if (!commonItems::DoesFolderExist(modPath))
	{
		Log(LogLevel::Warning) << "Mod path: " << modPath << " does not exist or can not be accessed!";
		return;
	}

	// Are there mods inside?
	std::vector<std::string> validModFiles;
	for (const auto& file: commonItems::GetAllFilesInFolder(modPath))
	{
		const auto lastDot = file.find_last_of('.');
		if (lastDot == std::string::npos)
			continue;
		const auto extension = file.substr(lastDot + 1, file.length() - lastDot - 1);
		if (extension != "mod")
			continue;
		validModFiles.emplace_back(file);
	}

	if (validModFiles.empty())
	{
		Log(LogLevel::Warning) << "No mod files could be found in " << modPath;
		return;
	}

	for (const auto& modFile: validModFiles)
	{
		Mod theMod(modPath + "/" + modFile);
		if (theMod.getName().empty())
		{
			Log(LogLevel::Warning) << "Mod at " << modPath + "/" + modFile << " has no defined name, skipping.";
			continue;
		}
		autolocatedMods.emplace_back(theMod);
	}

	// filter broken filenames from preloaded list.
	std::set<std::string> modNames;
	for (const auto& mod: autolocatedMods)
		modNames.insert(mod.getFileName());

	for (auto it = preloadedModFileNames.begin(); it != preloadedModFileNames.end();)
	{
		if (!modNames.count(*it))
		{
			it = preloadedModFileNames.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Configuration::setEnabledMods(const std::set<int>& selection)
{
	preloadedModFileNames.clear();
	for (auto counter = 0; counter < static_cast<int>(autolocatedMods.size()); counter++)
	{
		if (selection.count(counter))
		{
			preloadedModFileNames.insert(autolocatedMods[counter].getFileName());
		}
	}
}