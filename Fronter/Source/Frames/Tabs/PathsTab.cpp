#include "PathsTab.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <wx/filepicker.h>

PathsTab::PathsTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY)
{
}

void PathsTab::initializePaths()
{
	// Initialize a 2-row table
	wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 5);
	SetSizer(gridSizer);

	auto stSize = wxSize(75, -1);
	auto swSize = wxSize(-1, 300);
	auto tcSize = wxSize(150, -1);
	auto counter = 0;

	const auto userDir = getenv("USERPROFILE");
	const auto documentsDir = std::string(userDir) + R"(\Documents)";

	for (const auto& folder: configuration->getRequiredFolders())
	{
		if (!folder.second->isMandatory())
			continue;
		counter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, folder.second->getDisplayName(), wxDefaultPosition);

		std::string folderPath;
		if (folder.second->getSearchPathType() == "windowsUsersFolder")
			folderPath = documentsDir + '\\' + folder.second->getSearchPath();
		else if (folder.second->getSearchPathType() == "steamFolder")
		{
			const auto& possiblePath = getSteamInstallPath(folder.second->getSearchPathID());
			if (possiblePath)
			{
				folderPath = *possiblePath;
			}
		}

		wxDirPickerCtrl* dirPickerCtrl = new wxDirPickerCtrl(this, counter, folderPath, wxDirSelectorPromptStr, wxDefaultPosition, wxSize(650, wxDefaultCoord));
		dirPickerCtrl->Connect(wxEVT_DIRPICKER_CHANGED, (wxObjectEventFunction)&PathsTab::OnPathChanged, NULL, this);
		dirPickerCtrl->SetInitialDirectory(folderPath);
		folder.second->setID(counter);
		folder.second->setValue(folderPath);
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, NULL);
		GetSizer()->Add(dirPickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, NULL);
	}

	for (const auto& file: configuration->getRequiredFiles())
	{
		if (!file.second->isMandatory())
			continue;
		counter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, file.second->getDisplayName(), wxDefaultPosition);

		std::string filePath;
		std::string initialPath;

		if (file.second->getSearchPathType() == "windowsUsersFolder")
		{
			filePath = documentsDir + '\\' + file.second->getSearchPath() + '\\' + file.second->getFilename();
			initialPath = documentsDir + '\\' + file.second->getSearchPath();
		}
		else if (file.second->getSearchPathType() == "converterFolder")
		{
			wchar_t buf[256];
			GetCurrentDirectory(256, buf);
			auto currentDirectory = Utils::convertUTF16ToUTF8(std::wstring(buf)) + '\\';
			filePath = currentDirectory + file.second->getSearchPath() + '\\' + file.second->getFilename();
			initialPath = currentDirectory + file.second->getSearchPath();
		}

		wxFilePickerCtrl* filePickerCtrl = new wxFilePickerCtrl(this,
			 counter,
			 filePath,
			 wxFileSelectorPromptStr,
			 file.second->getAllowedExtension(),
			 wxDefaultPosition,
			 wxSize(650, wxDefaultCoord));
		filePickerCtrl->Connect(wxEVT_FILEPICKER_CHANGED, (wxObjectEventFunction)&PathsTab::OnPathChanged, NULL, this);
		filePickerCtrl->SetInitialDirectory(initialPath);
		file.second->setID(counter);
		file.second->setValue(filePath);
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, NULL);
		GetSizer()->Add(filePickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, NULL);
	}
}

std::optional<std::string> PathsTab::getSteamInstallPath(const std::string& steamID) const
{
	if (steamID.empty())
		return std::nullopt;

	wchar_t value[255];
	DWORD BufferSize = 8192;
	std::wstring registryPath = Utils::convertUTF8ToUTF16(R"(SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	const std::wstring installPath = Utils::convertUTF8ToUTF16(R"(InstallLocation)");

	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);

	if (value)
	{
		auto result = Utils::convertUTF16ToUTF8(std::wstring(value));
		if (!result.empty() && result.length() > 2)
		{
			return result;
		}
	}

	registryPath = Utils::convertUTF8ToUTF16(R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);

	if (value)
	{
		auto result = Utils::convertUTF16ToUTF8(std::wstring(value));
		if (!result.empty() && result.length() > 2)
		{
			return result;
		}
	}

	return std::nullopt;
}


void PathsTab::OnPathChanged(wxFileDirPickerEvent& evt)
{
	for (const auto& folder: configuration->getRequiredFolders())
		if (folder.second->getID() == evt.GetId())
			folder.second->setValue(evt.GetPath().ToStdString());
	for (const auto& file: configuration->getRequiredFiles())
		if (file.second->getID() == evt.GetId())
			file.second->setValue(evt.GetPath().ToStdString());
}

wxBEGIN_EVENT_TABLE(PathsTab, wxNotebookPage) wxEND_EVENT_TABLE()