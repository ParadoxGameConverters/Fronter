#include "PathsTab.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
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
	wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 5);
	SetSizer(gridSizer);

	auto userDir = std::getenv("USERPROFILE");
	std::string documentsDir;
	if (userDir)
	{
		documentsDir = std::string(userDir) + R"(\Documents)";
	}

	if (!userDir)
	{
		userDir = std::getenv("HOME");
		if (userDir)
		{
			documentsDir = std::string(userDir) + R"(/Documents)";
		}
	}
	for (const auto& folder: configuration->getRequiredFolders())
	{
		pickerCounter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, tr(folder.second->getDisplayName()), wxDefaultPosition);

		std::wstring folderPath;
		if (!folder.second->getValue().empty())
		{
			folderPath = commonItems::convertUTF8ToUTF16(folder.second->getValue());
		}
		else if (folder.second->getSearchPathType() == "windowsUsersFolder")
			folderPath = commonItems::convertUTF8ToUTF16(documentsDir + '\\' + folder.second->getSearchPath());
		else if (folder.second->getSearchPathType() == "steamFolder")
		{
			const auto& possiblePath = commonItems::getSteamInstallPath(folder.second->getSearchPathID());
			if (possiblePath)
			{
				folderPath = *possiblePath;
				if (!folder.second->getSearchPath().empty())
					folderPath += L"/" + commonItems::convertUTF8ToUTF16(folder.second->getSearchPath());
			}
		}
		else if (folder.second->getSearchPathType() == "direct")
			folderPath = commonItems::convertUTF8ToUTF16(folder.second->getSearchPath());

		if (!commonItems::DoesFolderExist(commonItems::UTF16ToUTF8(folderPath)))
			folderPath.clear();

		wxDirPickerCtrl* dirPickerCtrl =
			 new wxDirPickerCtrl(this, pickerCounter, folderPath, tr("BROWSE"), wxDefaultPosition, wxSize(650, wxDefaultCoord), wxFLP_USE_TEXTCTRL | wxFLP_SMALL);
		dirPickerCtrl->Bind(wxEVT_DIRPICKER_CHANGED, &PathsTab::OnPathChanged, this);
		dirPickerCtrl->SetInitialDirectory(wxString(folderPath));
		folder.second->setID(pickerCounter);
		folder.second->setValue(commonItems::UTF16ToUTF8(folderPath));
		// Intermezzo for mod detection
		if (!configuration->getAutoGenerateModsFrom().empty() && folder.second->getName() == configuration->getAutoGenerateModsFrom())
			configuration->autoLocateMods();
		// Carry on
		st->SetToolTip(tr(folder.second->getTooltip()));
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(dirPickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
	}

	for (const auto& file: configuration->getRequiredFiles())
	{
		pickerCounter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, tr(file.second->getDisplayName()), wxDefaultPosition);

		std::wstring filePath;
		std::wstring initialPath;

		if (!file.second->getValue().empty())
		{
			filePath = commonItems::convertUTF8ToUTF16(file.second->getValue());
			auto pos(filePath.find_last_of('\\'));
			initialPath = filePath.substr(0, pos + 1);
		}
		else if (file.second->getSearchPathType() == "windowsUsersFolder")
		{
			filePath = commonItems::convertUTF8ToUTF16(documentsDir + '\\' + file.second->getSearchPath() + '\\' + file.second->getFilename());
			initialPath = commonItems::convertUTF8ToUTF16(documentsDir + '\\' + file.second->getSearchPath() + '\\');
		}
		else if (file.second->getSearchPathType() == "converterFolder")
		{
			auto buf = commonItems::GetCurrentDirectoryWString();
#if defined __WIN32__
			auto currentDirectory = buf + '\\';
			filePath = currentDirectory + commonItems::convertUTF8ToUTF16(file.second->getSearchPath() + '\\' + file.second->getFilename());
			initialPath = currentDirectory + commonItems::convertUTF8ToUTF16(file.second->getSearchPath() + '\\');
#elif defined __linux
			auto currentDirectory = buf + '/';
			filePath = currentDirectory + commonItems::convertUTF8ToUTF16(file.second->getSearchPath() + '/' + file.second->getFilename());
			initialPath = currentDirectory + commonItems::convertUTF8ToUTF16(file.second->getSearchPath() + '/');
#endif
		}
		std::string allowedExtension;
#if defined __WIN32__
		allowedExtension = file.second->getAllowedExtension();
#endif

		if (!commonItems::DoesFileExist(commonItems::UTF16ToUTF8(filePath)) || !commonItems::DoesFolderExist(commonItems::UTF16ToUTF8(initialPath)))
		{
			filePath.clear();
			initialPath.clear();
		}

		wxFilePickerCtrl* filePickerCtrl = new wxFilePickerCtrl(this,
			 pickerCounter,
			 filePath,
			 tr("BROWSE"),
			 allowedExtension,
			 wxDefaultPosition,
			 wxSize(650, wxDefaultCoord),
			 wxFLP_USE_TEXTCTRL | wxFLP_SMALL);
		filePickerCtrl->Bind(wxEVT_FILEPICKER_CHANGED, &PathsTab::OnPathChanged, this);
		filePickerCtrl->SetInitialDirectory(wxString(initialPath));
		st->SetToolTip(tr(file.second->getTooltip()));
		file.second->setID(pickerCounter);
		file.second->setValue(commonItems::UTF16ToUTF8(filePath));
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(filePickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
	}
}


void PathsTab::OnPathChanged(wxFileDirPickerEvent& evt)
{
	for (const auto& folder: configuration->getRequiredFolders())
		if (folder.second->getID() == evt.GetId())
		{
			const auto validPath = commonItems::DoesFolderExist(commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring()));
			if (!validPath)
			{
				Log(LogLevel::Error) << "Cannot access folder: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring())
											<< " - Onedrive and similar symlink folders are not supported!";
				// Not bailing. We may not be able to access it but who knows, maybe converter can.
			}
			const auto result = commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
			folder.second->setValue(result);
			Log(LogLevel::Info) << folder.second->getName() << " set to: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
			// Intermezzo for mod detection
			if (!configuration->getAutoGenerateModsFrom().empty() && folder.second->getName() == configuration->getAutoGenerateModsFrom())
			{
				configuration->autoLocateMods();
				wxCommandEvent evt(wxEVT_UPDATEMODS);
				m_pParent->AddPendingEvent(evt);
			}
		}
	for (const auto& file: configuration->getRequiredFiles())
		if (file.second->getID() == evt.GetId())
		{
			if (!commonItems::DoesFileExist(commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring())))
			{
				Log(LogLevel::Error) << "Cannot access file: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring())
											<< " - Onedrive and similar symlink folders are not supported!";
			}
			std::string result = commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
			file.second->setValue(result);
			Log(LogLevel::Info) << file.second->getName() << " set to: " << commonItems::UTF16ToUTF8(evt.GetPath().ToStdWstring());
		}
}
