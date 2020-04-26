#include "Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

Configuration::Configuration::Configuration()
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
		Log(LogLevel::Warning) << "Configuration/fronter-configuration.txt not found!";
	}
	clearRegisteredKeywords();
	registerPreloadKeys();
	if (!converterFolder.empty() && fs::exists(fs::u8path(converterFolder + "/configuration.txt")))
	{
		parseFile(converterFolder + "/configuration.txt");
		Log(LogLevel::Info) << "Previous configuration located, preloaded selections.";
	}
	clearRegisteredKeywords();
}

void Configuration::Configuration::registerPreloadKeys()
{
	registerRegex("[a-zA-Z0-9_-]+", [this](const std::string& incomingKey, std::istream& theStream) {
		const commonItems::singleString valueStr(theStream);

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

Configuration::Configuration::Configuration(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void Configuration::Configuration::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("converterFolder", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		converterFolder = nameStr.getString();
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
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}

bool Configuration::Configuration::exportConfiguration() const
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
