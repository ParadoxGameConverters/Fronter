#include "PathsTab.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <wx/filepicker.h>
#include <filesystem>
#include <codecvt>
namespace fs = std::filesystem;


PathsTab::PathsTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
}

void PathsTab::initializePaths()
{
	// Initialize a 2-row table
	wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 5);
	SetSizer(gridSizer);

	const auto userDir = getenv("USERPROFILE");
	const auto documentsDir = std::string(userDir) + R"(\Documents)";

	for (const auto& folder: configuration->getRequiredFolders())
	{
		if (!folder.second->isMandatory())
			continue;
		pickerCounter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, folder.second->getDisplayName(), wxDefaultPosition);

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
			}
		}

		wxDirPickerCtrl* dirPickerCtrl =
			 new wxDirPickerCtrl(this, pickerCounter, folderPath, wxDirSelectorPromptStr, wxDefaultPosition, wxSize(650, wxDefaultCoord));
		dirPickerCtrl->Connect(wxEVT_DIRPICKER_CHANGED, (wxObjectEventFunction)&PathsTab::OnPathChanged, nullptr, this);
		dirPickerCtrl->SetInitialDirectory(wxString(folderPath));
		folder.second->setID(pickerCounter);
		folder.second->setValue(folderPath);
		st->SetToolTip(folder.second->getTooltip());
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(dirPickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
	}

	for (const auto& file: configuration->getRequiredFiles())
	{
		if (!file.second->isMandatory())
			continue;
		pickerCounter++;
		wxStaticText* st = new wxStaticText(this, wxID_ANY, file.second->getDisplayName(), wxDefaultPosition);

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
			wchar_t buf[256];
			GetCurrentDirectory(256, buf);
			auto currentDirectory = Utils::convertUTF16ToUTF8(std::wstring(buf)) + '\\';
			filePath = currentDirectory + file.second->getSearchPath() + '\\' + file.second->getFilename();
			initialPath = currentDirectory + file.second->getSearchPath() + '\\';
		}

		wxFilePickerCtrl* filePickerCtrl = new wxFilePickerCtrl(this,
			 pickerCounter,
			 filePath,
			 wxFileSelectorPromptStr,
			 file.second->getAllowedExtension(),
			 wxDefaultPosition,
			 wxSize(650, wxDefaultCoord));
		filePickerCtrl->Connect(wxEVT_FILEPICKER_CHANGED, (wxObjectEventFunction)&PathsTab::OnPathChanged, nullptr, this);
		filePickerCtrl->SetInitialDirectory(wxString(initialPath));
		st->SetToolTip(file.second->getTooltip());
		file.second->setID(pickerCounter);
		file.second->setValue(filePath);
		GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5, nullptr);
		GetSizer()->Add(filePickerCtrl, 0, wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5, nullptr);
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

	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, (PVOID)&value, &BufferSize);

	if (value)
	{
		auto result = Utils::convertUTF16ToUTF8(std::wstring(value));
		if (!result.empty() && result.length() > 2)
		{
			return result;
		}
	}

	registryPath = Utils::convertUTF8ToUTF16(R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, (PVOID)&value, &BufferSize);

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
		{			
			std::wstring theString = evt.GetPath().ToStdWstring();
			std::u16string u16str(theString.begin(), theString.end());
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
			std::string result = conversion.to_bytes(u16str);
			
			folder.second->setValue(result);
		}
	for (const auto& file: configuration->getRequiredFiles())
		if (file.second->getID() == evt.GetId())
		{
			std::wstring theString = evt.GetPath().ToStdWstring();
			std::u16string u16str(theString.begin(), theString.end());
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
			std::string result = conversion.to_bytes(u16str);
			
			file.second->setValue(result);
		}
}

wxBEGIN_EVENT_TABLE(PathsTab, wxNotebookPage)
wxEND_EVENT_TABLE()

