#include "ModCopier.h"
#include "CommonFunctions.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <chrono>
#include <filesystem>
#include <random>
namespace fs = std::filesystem;

wxDEFINE_EVENT(wxEVT_COPIERDONE, wxCommandEvent);

namespace
{
unsigned char random_char()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	return static_cast<unsigned char>(dis(gen));
}

std::string generate_hex(const unsigned int len)
{
	std::stringstream ss;
	for (unsigned int i = 0; i < len; i++)
	{
		const auto rc = random_char();
		std::stringstream hexStream;
		hexStream << std::hex << static_cast<int>(rc);
		auto hex = hexStream.str();
		ss << (hex.length() < 2 ? '0' + hex : hex);
	}
	return ss.str();
}

std::string generate_uuid()
{
	return generate_hex(4) + "-" + generate_hex(2) + "-" + generate_hex(2) + "-" + generate_hex(2) + "-" + generate_hex(6);
}
} // namespace

void* ModCopier::Entry()
{
	wxCommandEvent evt(wxEVT_COPIERDONE);
	Log(LogLevel::Notice) << "Mod Copying Started.";
	const auto& converterFolder = configuration->getConverterFolder();
	if (!commonItems::DoesFolderExist(converterFolder))
	{
		Log(LogLevel::Error) << "Copy failed - where is the converter?";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	if (!commonItems::DoesFolderExist(converterFolder + "/output"))
	{
		Log(LogLevel::Error) << "Copy failed - where is the converter's output folder?";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}

	const auto& requiredFolders = configuration->getRequiredFolders();
	const auto& folderItr = requiredFolders.find("targetGameModPath");
	if (folderItr == requiredFolders.end())
	{
		Log(LogLevel::Error) << "Copy failed - Target Folder isn't loaded!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	const auto& destinationFolder = folderItr->second->getValue();
	if (!commonItems::DoesFolderExist(destinationFolder))
	{
		Log(LogLevel::Error) << "Copy failed - Target Folder does not exist!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	const auto& options = configuration->getOptions();
	std::string targetName;
	for (const auto& option: options)
	{
		if (option->getName() == "output_name" && !option->getValue().empty())
		{
			targetName = option->getValue();
		}
	}
	const auto& requiredFiles = configuration->getRequiredFiles();
	if (targetName.empty())
	{
		const auto& fileItr = requiredFiles.find("SaveGame");
		if (fileItr == requiredFiles.end())
		{
			Log(LogLevel::Error) << "Copy failed - SaveGame is does not exist!";
			evt.SetInt(0);
			m_pParent->AddPendingEvent(evt);
			return nullptr;
		}
		auto saveGamePath = fileItr->second->getValue();
		if (saveGamePath.empty())
		{
			Log(LogLevel::Error) << "Copy Failed - save game path is empty, did we even convert anything?";
			evt.SetInt(0);
			m_pParent->AddPendingEvent(evt);
			return nullptr;
		}
		if (!commonItems::DoesFileExist(saveGamePath))
		{
			Log(LogLevel::Error) << "Copy Failed - save game does not exist, did we even convert anything?";
			evt.SetInt(0);
			m_pParent->AddPendingEvent(evt);
			return nullptr;
		}
		if (fs::is_directory(fs::u8path(saveGamePath)))
		{
			Log(LogLevel::Error) << "Copy Failed - Save game is a directory...";
			evt.SetInt(0);
			m_pParent->AddPendingEvent(evt);
			return nullptr;
		}
		saveGamePath = trimPath(saveGamePath);
		saveGamePath = normalizeStringPath(saveGamePath);
		const auto pos = saveGamePath.find_last_of('.');
		if (pos != std::string::npos)
			saveGamePath = saveGamePath.substr(0, pos);
		targetName = saveGamePath;
	}
	targetName = replaceCharacter(targetName, '-');
	targetName = replaceCharacter(targetName, ' ');
	targetName = commonItems::normalizeUTF8Path(targetName);

	if (!commonItems::DoesFolderExist(converterFolder + "/output/" + targetName))
	{
		Log(LogLevel::Error) << "Copy Failed - Could not find mod folder: " << converterFolder + "/output/" + targetName;
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}

	// for vic3 and onwards we need to skip .mod requirement

	bool vic3OnwardSkipModFile = false;
	if (commonItems::DoesFolderExist(converterFolder + "/output/" + targetName + "/.metadata"))
		vic3OnwardSkipModFile = true;
	if (!vic3OnwardSkipModFile && !commonItems::DoesFileExist(converterFolder + "/output/" + targetName + ".mod"))
	{
		Log(LogLevel::Error) << "Copy Failed - Could not find mod: " << converterFolder + "/output/" + targetName + ".mod";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	if (!fs::is_directory(fs::u8path(converterFolder + "/output/" + targetName)))
	{
		Log(LogLevel::Error) << "Copy Failed - Mod folder is not a directory: " << converterFolder + "/output/" + targetName;
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	if (!vic3OnwardSkipModFile && commonItems::DoesFileExist(destinationFolder + "/" + targetName + ".mod"))
	{
		Log(LogLevel::Info) << "Previous mod file found, deleting.";
		fs::remove(fs::u8path(destinationFolder + "/" + targetName + ".mod"));
	}
	if (commonItems::DoesFolderExist(destinationFolder + "/" + targetName))
	{
		Log(LogLevel::Info) << "Previous mod directory found, deleting.";
		commonItems::DeleteFolder(destinationFolder + "/" + targetName);
	}
	try
	{
		Log(LogLevel::Info) << "Copying mod to target location...";
		if (!vic3OnwardSkipModFile)
			commonItems::TryCopyFile(converterFolder + "/output/" + targetName + ".mod", destinationFolder + "/" + targetName + ".mod");
		commonItems::CopyFolder(converterFolder + "/output/" + targetName, destinationFolder + "/" + targetName);
	}
	catch (std::filesystem::filesystem_error& theError)
	{
		Log(LogLevel::Error) << theError.what();
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	Log(LogLevel::Notice) << "Mod successfully copied to: " << destinationFolder + "/" + targetName;

	createPlayset(destinationFolder, targetName, destinationFolder + "/" + targetName);

	evt.SetInt(1);
	m_pParent->AddPendingEvent(evt);
	return nullptr;
}

void ModCopier::createPlayset(const std::string& destModFolder, const std::string& targetName, const std::string& destModFolderPath)
{
	const auto gameDocsDirectory = destModFolder + "/..";
	if (!commonItems::DoesFolderExist(gameDocsDirectory))
	{
		Log(LogLevel::Warning) << "Couldn't resolve parent folder of " << destModFolder;
		return;
	}

	const auto latestDbFilePath = getLastUpdatedLauncherDbPath(gameDocsDirectory);
	if (latestDbFilePath.empty())
	{
		Log(LogLevel::Info) << "Launcher's database not found or this game doesn't have a launcher.";
		Log(LogLevel::Notice) << "All done. Have fun! -- Paradox Game Converters Team";
		return;
	}

	Log(LogLevel::Notice) << "Enabling Playset, please hold.";

	try
	{
		SQLite::Database db(latestDbFilePath, SQLite::OPEN_READWRITE);
		std::string playsetName = configuration->getName() + ": " + targetName;
		auto unixTimeMilliSeconds = std::chrono::system_clock::now().time_since_epoch().count();

		SQLite::Statement query(db, "SELECT COUNT(*) FROM playsets WHERE name = ?");
		query.bind(1, playsetName);
		query.executeStep();
		if (query.getColumn(0).getInt() > 0)
		{
			if (configuration->getOverwritePlayset())
			{
				deactivateCurrentPlayset(db);

				Log(LogLevel::Notice) << "Reactivating existing playset " << playsetName << ".";
				SQLite::Statement query2(db, "UPDATE playsets SET isActive=true, updatedOn = ? WHERE name = ?");
				query2.bind(1, unixTimeMilliSeconds);
				query2.bind(2, playsetName);
				query2.exec();
				Log(LogLevel::Notice) << "Have fun! -- Paradox Game Converters Team";
			}
			else
			{
				Log(LogLevel::Notice) << "Playset " + playsetName + " already exists, select it and play. Have fun!  -- Paradox Game Converters Team";
			}
		}
		else
		{
			deactivateCurrentPlayset(db);
			const auto playsetID = generate_uuid();
			SQLite::Statement query2(db,
				 "INSERT INTO playsets(id, name, isActive, isRemoved, hasNotApprovedChanges, createdOn) "
				 "VALUES(?, ?, true, false, false, ?)");
			query2.bind(1, playsetID);
			query2.bind(2, playsetName);
			query2.bind(3, unixTimeMilliSeconds);
			query2.exec();

			auto gameRegistryId = "mod/" + targetName + ".mod";
			auto modId = addModToDb(db, targetName, gameRegistryId, destModFolder);
			addModToPlayset(db, modId, playsetID);
			Log(LogLevel::Notice) << "Playset " + playsetName + " created, select it and play. Have fun! -- Paradox Game Converters Team";
		}
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Error) << e.what();
		Log(LogLevel::Notice) << "Adding mod/playset to the launcher failed. You need to do it yourself.";
	}
}

std::string ModCopier::getLastUpdatedLauncherDbPath(const std::string& gameDocsDirectory)
{
	const std::set<std::string> possibleDbFileNames = {"launcher-v2.sqlite", "launcher-v2_openbeta.sqlite"};
	fs::file_time_type lastAccess;
	std::string actualName;
	for (const auto& name: possibleDbFileNames)
	{
		auto path = std::filesystem::path(gameDocsDirectory + "/" + name);
		if (!commonItems::DoesFileExist(gameDocsDirectory + "/" + name))
			continue;
		if (lastAccess < last_write_time(path))
		{
			lastAccess = last_write_time(path);
			actualName = name;
		}
	}
	if (actualName.empty())
		return actualName;
	return gameDocsDirectory + "/" + actualName;
}

void ModCopier::deactivateCurrentPlayset(SQLite::Database& db)
{
	Log(LogLevel::Debug) << "Deactivating currently active playset.";
	SQLite::Statement query(db, "UPDATE playsets SET isActive=false");
	query.exec();
}

std::string ModCopier::addModToDb(SQLite::Database& db, const std::string& modName, const std::string& gameRegistryId, const std::string& dirPath)
{
	auto modID = generate_uuid();
	SQLite::Statement query(db,
		 "INSERT INTO mods(id, status, source, version, gameRegistryId, name, dirPath) "
		 "VALUES(?, 'ready_to_play', 'local', 1, ?, ?, ?)");
	query.bind(1, modID);
	query.bind(2, gameRegistryId);
	query.bind(3, modName);
	query.bind(4, dirPath);
	query.exec();

	return modID;
}

void ModCopier::addModToPlayset(SQLite::Database& db, const std::string& modID, const std::string& playsetID)
{
	SQLite::Statement query(db, "INSERT INTO playsets_mods(playsetId, modId) VALUES(?, ?)");
	query.bind(1, playsetID);
	query.bind(2, modID);
	query.exec();
}
