#include "Configuration.h"
#include "OSCompatibilityLayer.h"
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

bool Configuration::Configuration::copyMod() const
{
	if (!fs::exists(converterFolder))
	{
		Log(LogLevel::Error) << "Copy failed - where is the converter?";
		return false;
	}
	if (!fs::exists(converterFolder + "/output"))
	{
		Log(LogLevel::Error) << "Copy failed - where is the converter's output folder?";
		return false;		
	}
	
	const auto& folderItr = requiredFolders.find("targetGameModPath");
	if (folderItr == requiredFolders.end())
	{
		Log(LogLevel::Error) << "Copy failed - Target Folder isn't loaded!";
		return false;	
	}
	const auto& destinationFolder = folderItr->second->getValue();
	if (!fs::exists( fs::u8path(destinationFolder)))
	{
		Log(LogLevel::Error) << "Copy failed - Target Folder does not exist!";
		return false;	
	}
	std::string targetName;
	for (const auto& option: options)
	{
		if (option->getName() == "output_name" && !option->getValue().empty())
		{
			targetName = option->getValue();
		}
	}
	if (targetName.empty())
	{
		const auto& fileItr = requiredFiles.find("SaveGame");
		if (fileItr == requiredFiles.end())
		{
			Log(LogLevel::Error) << "Copy failed - SaveGame is does not exist!";
			return false;	
		}
		std::string saveGamePath = fileItr->second->getValue();
		if (saveGamePath.empty())
		{
			Log(LogLevel::Error) << "Copy Failed - save game path is empty, did we even convert anything?";
			return false;	
		}
		if (!fs::exists(fs::u8path(saveGamePath)))
		{
			Log(LogLevel::Error) << "Copy Failed - save game does not exist, did we even convert anything?";
			return false;	
		}
		if (fs::is_directory(fs::u8path(saveGamePath)))
		{
			Log(LogLevel::Error) << "Copy Failed - Save game is a directory...";
			return false;	
		}
		saveGamePath = trimPath(saveGamePath);
		saveGamePath = normalizeStringPath(saveGamePath);
		auto pos = saveGamePath.find_last_of('.');
		if (pos != std::string::npos) saveGamePath = saveGamePath.substr(0, pos);
		targetName = saveGamePath;
	}
	if (!fs::exists(fs::u8path(converterFolder + "/output/" + targetName + ".mod")))
	{
		Log(LogLevel::Error) << "Copy Failed - Cound not find mod: " << converterFolder + "/output/" + targetName + ".mod";
		return false;	
	}
	if (!fs::exists(fs::u8path(converterFolder + "/output/" + targetName)))
	{
		Log(LogLevel::Error) << "Copy Failed - Cound not find mod folder: " << converterFolder + "/output/" + targetName;
		return false;
	}
	if (!fs::is_directory(fs::u8path(converterFolder + "/output/" + targetName)))
	{
		Log(LogLevel::Error) << "Copy Failed - Mod folder is not a directory: " << converterFolder + "/output/" + targetName;
		return false;
	}
	if (fs::exists(fs::u8path(destinationFolder + "/" + targetName + ".mod")))
		fs::remove(fs::u8path(destinationFolder + "/" + targetName + ".mod"));
	if (fs::exists(fs::u8path(destinationFolder + "/" + targetName)))
		fs::remove_all(fs::u8path(destinationFolder + "/" + targetName));		
	try
	{
		fs::copy(fs::u8path(converterFolder + "/output/" + targetName + ".mod"), fs::u8path(destinationFolder + "/" + targetName + ".mod"));
		fs::copy(fs::u8path(converterFolder + "/output/" + targetName), fs::u8path(destinationFolder + "/" + targetName), std::filesystem::copy_options::recursive);
	}
	catch (std::filesystem::filesystem_error& theError)
	{
		Log(LogLevel::Error) << theError.what();
		return false;
	}
	Log(LogLevel::Info) << "Mod successfully copied to: " << destinationFolder + "/" + targetName;
	return true;	
}

std::string Configuration::Configuration::getSecondTailSource() const
{
	return converterFolder + "/log.txt";
}

Configuration::LogMessage Configuration::Configuration::sliceMessage(const std::string& message)
{
	LogMessage logMessage;
	const auto posOpen = message.find_first_of('[');
	if (!(posOpen >= 20 && posOpen <= 23))
	{
		// This is not a standard message. File as info ad verbatim.
		logMessage.message = message;
		return logMessage;
	}
	const auto posClose = message.find_first_of(']');
	if (posClose == std::string::npos)
	{
		// something's very wrong with this message.
		logMessage.message = message;
		return logMessage;		
	}
	const auto logLevel = message.substr(posOpen + 1, posClose - posOpen - 1);
	if (logLevel == "INFO") logMessage.logLevel = LogLevel::Info;
	else if (logLevel == "DEBUG") logMessage.logLevel = LogLevel::Debug;
	else if (logLevel == "WARNING") logMessage.logLevel = LogLevel::Warning;
	else if (logLevel == "ERROR") logMessage.logLevel = LogLevel::Error;
	logMessage.timestamp = message.substr(0, 19);
	logMessage.message = message.substr(posClose + 2, message.length());
	return logMessage;
}

std::string Configuration::Configuration::normalizeStringPath(const std::string& stringPath)
{
	std::string toReturn = Utils::normalizeUTF8Path(stringPath);
	toReturn = replaceCharacter(toReturn, '-');
	toReturn = replaceCharacter(toReturn, ' ');

	return toReturn;
}

std::string Configuration::Configuration::replaceCharacter(std::string fileName, char character)
{
	auto position = fileName.find_first_of(character);
	while (position != std::string::npos)
	{
		fileName.replace(position, 1, "_");
		position = fileName.find_first_of(character);
	}

	return fileName;
}

std::string Configuration::Configuration::trimPath(const std::string& fileName)
{
	int lastSlash = fileName.find_last_of("\\");
	auto trimmedFileName = fileName.substr(lastSlash + 1, fileName.length());
	lastSlash = trimmedFileName.find_last_of("/");
	trimmedFileName = trimmedFileName.substr(lastSlash + 1, trimmedFileName.length());
	return trimmedFileName;
}
