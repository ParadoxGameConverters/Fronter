#include "ModCopier.h"
#include "Log.h"
#include <filesystem>
namespace fs = std::filesystem;
#include "CommonFunctions.h"
#include "OSCompatibilityLayer.h"

wxDEFINE_EVENT(wxEVT_COPIERDONE, wxCommandEvent);

void* ModCopier::Entry()
{
	wxCommandEvent evt(wxEVT_COPIERDONE);
	Log(LogLevel::Info) << "Mod Copying Started.";
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
	if (!commonItems::DoesFileExist(converterFolder + "/output/" + targetName + ".mod"))
	{
		Log(LogLevel::Error) << "Copy Failed - Cound not find mod: " << converterFolder + "/output/" + targetName + ".mod";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	if (!commonItems::DoesFolderExist(converterFolder + "/output/" + targetName))
	{
		Log(LogLevel::Error) << "Copy Failed - Cound not find mod folder: " << converterFolder + "/output/" + targetName;
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
	if (commonItems::DoesFileExist(destinationFolder + "/" + targetName + ".mod"))
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
	Log(LogLevel::Info) << "Mod successfully copied to: " << destinationFolder + "/" + targetName;
	evt.SetInt(1);
	m_pParent->AddPendingEvent(evt);
	return nullptr;
}
