#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>
#include "../../ConfigurationParser/Configuration.h"
#include <wx/filepicker.h>

class PathsTab : public wxNotebookPage
{
public:
	PathsTab(wxWindow* parent);

	[[nodiscard]] const auto& getTabName() const { return tabName; }

	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializePaths();
	
private:
	[[nodiscard]] std::optional<std::string> getSteamInstallPath(const std::string& steamID) const;

	void OnPathChanged(wxFileDirPickerEvent& evt);
	void OnChooseDir(wxCommandEvent& evt);
	
	std::string tabName = "Paths";
   wxDECLARE_EVENT_TABLE();

	std::shared_ptr<Configuration::Configuration> configuration;
};
