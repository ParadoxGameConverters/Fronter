#include "PathsTab.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <ranges>
#include <wx/filepicker.h>
namespace fs = std::filesystem;
#define tr localization->translate
#include <cstdlib>

wxDEFINE_EVENT(wxEVT_UPDATEMODS, wxCommandEvent);

PathsTab::PathsTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	m_pParent = parent;
}

void PathsTab::initializePaths()
{
	// Initialize a 2-row table
	auto* gridSizer = new wxFlexGridSizer(2, 2, 5);
	SetSizer(gridSizer);

	auto* userDir = std::getenv("USERPROFILE");
	std::filesystem::path documentsDir;
	if (userDir)
	{
		documentsDir = std::filesystem::path(userDir) / "Documents";
	}

	if (!userDir)
	{
		userDir = std::getenv("HOME");
		if (userDir)
		{
			documentsDir = std::filesystem::path(userDir) / "Documents";
		}
	}
	for (const auto& folder: configuration->getRequiredFolders() | std::ranges::views::values)
	{
		pickerCounter++;
		auto* st = new wxStaticText(this, wxID_ANY, tr(folder->getDisplayName()), wxDefaultPosition);

		std::filesystem::path folderPath;
		if (!folder->getValue().empty())
		{
			folderPath = folder->getValue();
		}
		else if (folder->getSearchPathType() == "windowsUsersFolder")
			folderPath = documentsDir / folder->getSearchPath();
		else if (folder->getSearchPathType() == "steamFolder")
		{
			const auto& possiblePath = commonItems::getSteamInstallPath(folder->getSearchPathID());
			if (possiblePath)
			{
				folderPath = *possiblePath;
				if (!folder->getSearchPath().empty())
					folderPath /= folder->getSearchPath();
			}
		}
		else if (folder->getSearchPathType() == "direct")
			folderPath = folder->getSearchPath();

		if (!commonItems::DoesFolderExist(folderPath))
			folderPath.clear();

		auto* dirPickerCtrl = new wxDirPickerCtrl(this,
			 pickerCounter,
			 folderPath.string(),
			 tr("BROWSE"),
			 wxDefaultPosition,
			 wxSize(650, wxDefaultCoord),
			 wxFLP_USE_TEXTCTRL | wxFLP_SMALL);
		dirPickerCtrl->Bind(wxEVT_DIRPICKER_CHANGED, &PathsTab::OnPathChanged, this);
		dirPickerCtrl->SetInitialDirectory(wxString(folderPath.string()));
		folder->setID(pickerCounter);
		folder->setValue(folderPath.string());
		// Intermezzo for mod detection
		if (!configuration->getAutoGenerateModsFrom().empty() && folder->getName() == configuration->getAutoGenerateModsFrom())
			configuration->autoLocateMods();
		// Carry on
		st->SetToolTip(tr(folder->getTooltip()));
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(dirPickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
	}

	for (const auto& file: configuration->getRequiredFiles() | std::ranges::views::values)
	{
		pickerCounter++;
		auto* st = new wxStaticText(this, wxID_ANY, tr(file->getDisplayName()), wxDefaultPosition);

		std::filesystem::path filePath;
		std::filesystem::path initialPath;

		if (!file->getValue().empty())
		{
			filePath = file->getValue();
			initialPath = filePath.parent_path();
		}
		else if (file->getSearchPathType() == "windowsUsersFolder")
		{
			filePath = documentsDir / file->getSearchPath() / file->getFilename();
			initialPath = documentsDir / file->getSearchPath();
		}
		else if (file->getSearchPathType() == "converterFolder")
		{
			auto currentDirectory = std::filesystem::current_path();
			filePath = currentDirectory / file->getSearchPath() / file->getFilename();
			initialPath = currentDirectory / file->getSearchPath();
		}
		std::string allowedExtension;
#if defined __WIN32__
		allowedExtension = file->getAllowedExtension();
#endif

		if (!commonItems::DoesFileExist(filePath) || !commonItems::DoesFolderExist(initialPath))
		{
			filePath.clear();
			initialPath.clear();
		}

		auto* filePickerCtrl = new wxFilePickerCtrl(this,
			 pickerCounter,
			 filePath.string(),
			 tr("BROWSE"),
			 allowedExtension,
			 wxDefaultPosition,
			 wxSize(650, wxDefaultCoord),
			 wxFLP_USE_TEXTCTRL | wxFLP_SMALL);
		filePickerCtrl->Bind(wxEVT_FILEPICKER_CHANGED, &PathsTab::OnPathChanged, this);
		filePickerCtrl->SetInitialDirectory(wxString(initialPath.string()));
		st->SetToolTip(tr(file->getTooltip()));
		file->setID(pickerCounter);
		file->setValue(filePath.string());
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(filePickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
	}
}


void PathsTab::OnPathChanged(wxFileDirPickerEvent& evt)
{
	for (const auto& folder: configuration->getRequiredFolders() | std::ranges::views::values)
		if (folder->getID() == evt.GetId())
		{
			const auto validPath = commonItems::DoesFolderExist(commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring()));
			if (!validPath)
			{
				Log(LogLevel::Error) << "Cannot access folder: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring())
											<< " - Onedrive and similar symlink folders are not supported!";
				// Not bailing. We may not be able to access it but who knows, maybe converter can.
			}
			const auto result = commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
			folder->setValue(result);
			Log(LogLevel::Info) << folder->getName() << " set to: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
			// Intermezzo for mod detection
			if (!configuration->getAutoGenerateModsFrom().empty() && folder->getName() == configuration->getAutoGenerateModsFrom())
			{
				configuration->autoLocateMods();
				wxCommandEvent evt(wxEVT_UPDATEMODS);
				m_pParent->AddPendingEvent(evt);
			}
		}
	for (const auto& file: configuration->getRequiredFiles() | std::ranges::views::values)
		if (file->getID() == evt.GetId())
		{
			if (!commonItems::DoesFileExist(commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring())))
			{
				Log(LogLevel::Error) << "Cannot access file: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring())
											<< " - Onedrive and similar symlink folders are not supported!";
			}
			auto result = commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
			file->setValue(result);
			Log(LogLevel::Info) << file->getName() << " set to: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
		}
}
