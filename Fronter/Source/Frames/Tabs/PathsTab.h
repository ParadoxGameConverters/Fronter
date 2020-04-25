#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../ConfigurationParser/Configuration.h"
#include <wx/filepicker.h>
#include <wx/notebook.h>

class PathsTab: public wxNotebookPage
{
  public:
	PathsTab(wxWindow* parent);

	[[nodiscard]] const auto& getTabName() const { return tabName; }

	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializePaths();

  private:
	[[nodiscard]] std::optional<std::string> getSteamInstallPath(const std::string& steamID) const;

	void OnPathChanged(wxFileDirPickerEvent& evt);

	int pickerCounter = 0;
	std::string tabName = "Paths";
	wxDECLARE_EVENT_TABLE();
	std::shared_ptr<Configuration::Configuration> configuration;
};
