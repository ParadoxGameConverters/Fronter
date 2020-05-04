#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

Configuration::Configuration(const std::string& language)
{
	useLanguage(language);
	std::ofstream clearLog("log.txt", std::ofstream::trunc);
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
		const commonItems::singleString valueStr(theStream);
		if (incomingKey == "configuration")
		{
			Log(LogLevel::Warning) << "You have an old configuration file. Preload will not be possible.";
			return;
		}
		for (const auto& folder: requiredFolders)
		{
			if (folder.first == incomingKey)
				folder.second->setValue(valueStr.getString());
		}
		for (const auto& file: requiredFiles)
		{
			if (file.first == incomingKey)
				file.second->setValue(valueStr.getString());
		}
		for (const auto& option: options)
		{
			if (option->getName() == incomingKey)
				option->setValue(valueStr.getString());
		}
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}

Configuration::Configuration(std::istream& theStream, const std::string& language)
{
	useLanguage(language);
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void Configuration::registerKeys()
{
	registerKeyword("supportedLocLanguages", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList locList(theStream);
		const auto& theList = locList.getStrings();
		const auto& langItr = std::find(theList.begin(), theList.end(), setLanguage);
		if (langItr == theList.end())
			setLanguage = "english";
	});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("converterFolder", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		converterFolder = nameStr.getString();
	});
	registerKeyword("displayName_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		displayName = nameStr.getString();
	});
	registerKeyword("sourceGame_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString gameStr(theStream);
		sourceGame = gameStr.getString();
	});
	registerKeyword("targetGame_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString gameStr(theStream);
		targetGame = gameStr.getString();
	});
	registerKeyword("requiredFolder", [this](const std::string& unused, std::istream& theStream) {
		auto newFolder = std::make_shared<RequiredFolder>(theStream, setLanguage);
		if (!newFolder->getName().empty())
			requiredFolders.insert(std::pair(newFolder->getName(), newFolder));
		else
			Log(LogLevel::Error) << "Required Folder has no mandatory field: name!";
	});
	registerKeyword("requiredFile", [this](const std::string& unused, std::istream& theStream) {
		auto newFile = std::make_shared<RequiredFile>(theStream, setLanguage);
		if (!newFile->getName().empty())
			requiredFiles.insert(std::pair(newFile->getName(), newFile));
		else
			Log(LogLevel::Error) << "Required File has no mandatory field: name!";
	});
	registerKeyword("option", [this](const std::string& unused, std::istream& theStream) {
		optionCounter++;
		auto newOption = std::make_shared<Option>(theStream, optionCounter, setLanguage);
		options.emplace_back(newOption);
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
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
	for (const auto& option: options)
	{
		confFile << option->getName() << " = \"" << option->getValue() << "\"\n";
	}
	confFile.close();
	return true;
}

std::string Configuration::getSecondTailSource() const
{
	return converterFolder + "/log.txt";
}

void Configuration::useLanguage(const std::string& language)
{
	if (!language.empty())
		setLanguage = language;
	else
		setLanguage = "english";
}