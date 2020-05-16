#include "PathsTab.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <wx/filepicker.h>
namespace fs = std::filesystem;
#include "../../Utils/OSFunctions.h"
#define tr localization->translate
#include <cstdlib>

PathsTab::PathsTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
}

void PathsTab::initializePaths()
{
    Log(LogLevel::Debug) << "paths 1";
	// Initialize a 2-row table
	wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 5);
	SetSizer(gridSizer);
    Log(LogLevel::Debug) << "paths 2";

	auto userDir = std::getenv("USERPROFILE");
	std::string documentsDir;
	if (userDir)
	{
        Log(LogLevel::Debug) << userDir;
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
    Log(LogLevel::Debug) << "paths 3";

	for (const auto& folder: configuration->getRequiredFolders())
	{
		if (!folder.second->isMandatory())
			continue;
		pickerCounter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, tr(folder.second->getDisplayName()), wxDefaultPosition);

    Log(LogLevel::Debug) << "paths 4";
		std::string folderPath;
		if (!folder.second->getValue().empty())
		{
			folderPath = folder.second->getValue();
		}
		else if (folder.second->getSearchPathType() == "windowsUsersFolder")
			folderPath = documentsDir + '\\' + folder.second->getSearchPath();
		else if (folder.second->getSearchPathType() == "steamFolder")
		{
			const auto& possiblePath = getSteamInstallPath(folder.second->getSearchPathID());
			if (possiblePath)
			{
				folderPath = *possiblePath;
				if (!folder.second->getSearchPath().empty())
					folderPath += "/" + folder.second->getSearchPath();
			}
		}
		else if (folder.second->getSearchPathType() == "direct")
			folderPath = folder.second->getSearchPath();
    Log(LogLevel::Debug) << "paths 5";

		wxDirPickerCtrl* dirPickerCtrl =
			 new wxDirPickerCtrl(this, pickerCounter, folderPath, tr("BROWSE"), wxDefaultPosition, wxSize(650, wxDefaultCoord), wxFLP_USE_TEXTCTRL | wxFLP_SMALL);
		dirPickerCtrl->Bind(wxEVT_DIRPICKER_CHANGED, &PathsTab::OnPathChanged, this);
		dirPickerCtrl->SetInitialDirectory(wxString(folderPath));
		folder.second->setID(pickerCounter);
		folder.second->setValue(folderPath);
		st->SetToolTip(tr(folder.second->getTooltip()));
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(dirPickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
	}
    Log(LogLevel::Debug) << "paths 6";

	for (const auto& file: configuration->getRequiredFiles())
	{
		if (!file.second->isMandatory())
			continue;
		pickerCounter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, tr(file.second->getDisplayName()), wxDefaultPosition);
    Log(LogLevel::Debug) << "paths 7";

		std::string filePath;
		std::string initialPath;

		if (!file.second->getValue().empty())
		{
			filePath = file.second->getValue();
			auto pos(filePath.find_last_of('\\'));
			initialPath = filePath.substr(0, pos + 1);
		}
		else if (file.second->getSearchPathType() == "windowsUsersFolder")
		{
			filePath = documentsDir + '\\' + file.second->getSearchPath() + '\\' + file.second->getFilename();
			initialPath = documentsDir + '\\' + file.second->getSearchPath() + '\\';
		}
		else if (file.second->getSearchPathType() == "converterFolder")
		{
			auto buf = Utils::GetCurrentDirectory();
#if defined __WIN32__
			auto currentDirectory = Utils::convertUTF16ToUTF8(std::wstring(buf)) + '\\';
#elif defined __linux
			auto currentDirectory = buf + '/';
#endif
			filePath = currentDirectory + file.second->getSearchPath() + '\\' + file.second->getFilename();
			initialPath = currentDirectory + file.second->getSearchPath() + '\\';
		}
    Log(LogLevel::Debug) << "paths 8";

		wxFilePickerCtrl* filePickerCtrl = new wxFilePickerCtrl(this,
			 pickerCounter,
			 filePath,
			 tr("BROWSE"),
			 file.second->getAllowedExtension(),
			 wxDefaultPosition,
			 wxSize(650, wxDefaultCoord),
			 wxFLP_USE_TEXTCTRL | wxFLP_SMALL);
		filePickerCtrl->Bind(wxEVT_FILEPICKER_CHANGED, &PathsTab::OnPathChanged, this);
		filePickerCtrl->SetInitialDirectory(wxString(initialPath));
		st->SetToolTip(tr(file.second->getTooltip()));
		file.second->setID(pickerCounter);
		file.second->setValue(filePath);
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(filePickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
	}
    Log(LogLevel::Debug) << "paths out";
}


void PathsTab::OnPathChanged(wxFileDirPickerEvent& evt)
{
	for (const auto& folder: configuration->getRequiredFolders())
		if (folder.second->getID() == evt.GetId())
		{
			std::string result = UTF16ToUTF8(evt.GetPath().ToStdWstring());
			folder.second->setValue(result);
		}
	for (const auto& file: configuration->getRequiredFiles())
		if (file.second->getID() == evt.GetId())
		{
			std::string result = UTF16ToUTF8(evt.GetPath().ToStdWstring());
			file.second->setValue(result);
		}
}
